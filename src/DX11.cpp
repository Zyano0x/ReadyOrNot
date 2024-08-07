#include "pch.h"

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
		g_Menu->m_IsShowMenu = !g_Menu->m_IsShowMenu;

	if (GetKeyPress(VK_HOME, false))
		g_Console->ToggleConsole();

	if (g_Menu->m_IsShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler((HWND)g_DX11Window->WndProc, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc((WNDPROC)g_DX11Window->WndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT DX11Window::PresentHook(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags)
{
	g_DX11Window->Overlay(SwapChain);

	return g_DX11Window->oPresent(SwapChain, SyncInterval, Flags);
}

HRESULT DX11Window::ResizeHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	SwapChain = SwapChain;
	g_DX11Window->RenderTargetView->Release();
	g_DX11Window->RenderTargetView = nullptr;

	HRESULT Result = g_DX11Window->oResizeBuffers(SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D*), (LPVOID*)&BackBuffer);
	if (BackBuffer)
	{
		g_DX11Window->Device->CreateRenderTargetView(BackBuffer, 0, &g_DX11Window->RenderTargetView);
		BackBuffer->Release();
	}

	if (g_DX11Window->InitImGui)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(Width), static_cast<float>(Height));
	}

	return Result;
}

void DX11Window::HookD3D()
{
	PresentFn = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();
	ResizeBuffersFn = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 8B FA"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();

	Hooking::CreateHook(reinterpret_cast<LPVOID>(PresentFn), &PresentHook, reinterpret_cast<LPVOID*>(&oPresent));
	Hooking::CreateHook(reinterpret_cast<LPVOID>(ResizeBuffersFn), &ResizeHook, reinterpret_cast<LPVOID*>(&oResizeBuffers));
}

void DX11Window::UnhookD3D()
{
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

	Hooking::DisableHook(reinterpret_cast<LPVOID>(PresentFn));
	Hooking::DisableHook(reinterpret_cast<LPVOID>(ResizeBuffersFn));

	Device->Release();
	DeviceContext->Release();
	RenderTargetView->Release();
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
	ImGui::GetIO().MouseDrawCursor = g_Menu->m_IsShowMenu;

	g_Game->Setup();
	g_Game->Visual();
	g_Game->Aimbot();
	g_Game->Misc();
	g_Menu->Draw();

	ImGui::EndFrame();
	ImGui::Render();
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Initialize()
{
	g_Console->Initialize(skCrypt("Debug Console"));
	g_DX11Window->HookD3D();
	g_Game->Hook();
}

void Deallocate()
{
	g_Console->DestroyConsole();
	g_DX11Window->UnhookD3D();
	g_Game->UnHook();
	g_Hooking->Shutdown();

	g_DX11Window.release();
	g_Console.release();
	g_Menu.release();
	g_Game.release();
	g_Hooking.release();
}