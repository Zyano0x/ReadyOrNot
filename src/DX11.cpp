#include "pch.h"

DX11Window g_DX11Window;

DX11Window::DX11Window() {}

DX11Window::~DX11Window() {}

LRESULT DX11Window::WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		VirtualKeys[VK_LBUTTON].bKey = true;
		break;

	case WM_LBUTTONUP:
		VirtualKeys[VK_LBUTTON].bKey = false;
		break;

	case WM_RBUTTONDOWN:
		VirtualKeys[VK_RBUTTON].bKey = true;
		break;

	case WM_RBUTTONUP:
		VirtualKeys[VK_RBUTTON].bKey = false;
		break;

	case WM_KEYDOWN:
		VirtualKeys[wParam].bKey = true;
		break;

	case WM_KEYUP:
		VirtualKeys[wParam].bKey = false;
		break;
	}

	if (GetKeyPress(VK_INSERT, false))
		Menu::m_IsShowMenu = !Menu::m_IsShowMenu;

	if (Menu::m_IsShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler((HWND)g_DX11Window.WndProc, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc((WNDPROC)g_DX11Window.WndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT DX11Window::PresentHook(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags)
{
	g_DX11Window.Overlay(SwapChain);
	return g_DX11Window.Present(SwapChain, SyncInterval, Flags);
}

HRESULT DX11Window::ResizeHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	SwapChain = SwapChain;
	g_DX11Window.RenderTargetView->Release();
	g_DX11Window.RenderTargetView = nullptr;

	HRESULT Result = g_DX11Window.ResizeBuffers(SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D*), (LPVOID*)&BackBuffer);
	if (BackBuffer)
	{
		g_DX11Window.Device->CreateRenderTargetView(BackBuffer, 0, &g_DX11Window.RenderTargetView);
		BackBuffer->Release();
	}

	if (g_DX11Window.InitImGui)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(Width), static_cast<float>(Height));
	}

	return Result;
}

void DX11Window::Overlay(IDXGISwapChain* SwapChain)
{
	if (!InitImGui)
	{
		if (SUCCEEDED(SwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Device)))
		{
			Device->GetImmediateContext(&DeviceContext);
			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			SwapChain->GetDesc(&SwapChainDesc);
			hWnd = SwapChainDesc.OutputWindow;

			ID3D11Texture2D* Buffer;
			SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Buffer);
			Device->CreateRenderTargetView(Buffer, NULL, &RenderTargetView);
			Buffer->Release();

			WndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProcHook);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.IniFilename = nullptr;

			io.Fonts->AddFontFromMemoryTTF(RudaFont, sizeof(RudaFont), 16.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplDX11_Init(Device, DeviceContext);
			ImGui_ImplDX11_CreateDeviceObjects();

			if (strcpy_s(szDllDirectory, sizeof(szDllDirectory), "C:\\RC\\") == 0)
				CreateDirectoryA(szDllDirectory, NULL);

			BotInitSettings();
			BotLoadSettings();

			InitImGui = true;
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = Menu::m_IsShowMenu;

	spoof_call(Game::Setup);
	spoof_call(Game::Visual);
	spoof_call(Game::Aimbot);
	spoof_call(Game::Misc);
	spoof_call(Menu::Draw);

	ImGui::EndFrame();
	ImGui::Render();
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DX11Window::Initialize(HMODULE dwModule)
{
	UNREFERENCED_PARAMETER(dwModule);

	g_Console.Initialize(skCrypt("Debug Console"), true);
	g_Game.Initilize();
	g_Hooking.Initialize();

	uint64_t PresentAddr = (uint64_t)(LI_FN(GetModuleHandleW)(skCrypt(L"GameOverlayRenderer64.dll"))) + 0x149BE0;
	uint64_t ResizeBuffersAddr = (uint64_t)(LI_FN(GetModuleHandleW)(skCrypt(L"GameOverlayRenderer64.dll"))) + 0x149BE8;

	IDXGISwapChainPresent* _Present = (IDXGISwapChainPresent*)PresentAddr;
	g_DX11Window.Present = *_Present;

	IDXGISwapChainResizeBuffers* _ResizeBuffers = (IDXGISwapChainResizeBuffers*)ResizeBuffersAddr;
	g_DX11Window.ResizeBuffers = *_ResizeBuffers;

	_InterlockedExchangePointer((volatile PVOID*)PresentAddr, PresentHook);
	_InterlockedExchangePointer((volatile PVOID*)ResizeBuffersAddr, ResizeHook);

	/*uint64_t hkPresent_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();
	uint64_t hkResizeBuffers_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 8B FA"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();
	uint64_t CreateHook_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 57 48 83 EC ? 33 C0 48 89 44 24"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();

	__int64(__fastcall * CreateHook)(unsigned __int64 pFuncAddress, __int64 pDetourFuncAddress, unsigned __int64* pOriginalFuncAddressOut, int a4);
	CreateHook = (decltype(CreateHook))CreateHook_Sig;
	CreateHook(hkPresent_Sig, (__int64)&PresentHook, (unsigned __int64*)&g_DX11Window.Present, 1);
	CreateHook(hkResizeBuffers_Sig, (__int64)&ResizeHook, (unsigned __int64*)&g_DX11Window.ResizeBuffers, 1);*/
}