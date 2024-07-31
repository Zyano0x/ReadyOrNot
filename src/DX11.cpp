#include "pch.h"

static uintx_t* MethodsTable = NULL;

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

	if (g_Engine->GetKeyPress(VK_INSERT, false))
		Menu::m_IsShowMenu = !Menu::m_IsShowMenu;

	if (Menu::m_IsShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler((HWND)g_DX11Window->WndProc, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc((WNDPROC)g_DX11Window->WndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT DX11Window::PresentHook(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags)
{
	g_DX11Window->Overlay(SwapChain);
	return g_DX11Window->Present(SwapChain, SyncInterval, Flags);
}

HRESULT DX11Window::ResizeHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	SwapChain = SwapChain;
	g_DX11Window->RenderTargetView->Release();
	g_DX11Window->RenderTargetView = nullptr;

	HRESULT Result = g_DX11Window->ResizeBuffers(SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

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
	spoof_call(Game::Misc);
	spoof_call(Menu::Draw);

	ImGui::EndFrame();
	ImGui::Render();
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool DX11Window::InitWindow()
{
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"MJ";
	WindowClass.hIconSm = NULL;
	RegisterClassEx(&WindowClass);
	WindowHwnd = CreateWindow(WindowClass.lpszClassName, skCrypt(L"DX11 Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
	if (WindowHwnd == NULL) {
		return FALSE;
	}
	return TRUE;
}

bool DX11Window::DeleteWindow()
{
	DestroyWindow(WindowHwnd);
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
	if (WindowHwnd != NULL) {
		return false;
	}
	return true;
}

bool DX11Window::GetDX11Context()
{
	if (!InitWindow())
		return false;

	HMODULE D3D11Module = GetModuleHandleW(skCrypt(L"d3d11.dll"));

	D3D_FEATURE_LEVEL FeatureLevel;
	const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_RATIONAL RefreshRate;
	RefreshRate.Numerator = 60;
	RefreshRate.Denominator = 1;

	DXGI_MODE_DESC BufferDesc;
	BufferDesc.Width = 100;
	BufferDesc.Height = 100;
	BufferDesc.RefreshRate = RefreshRate;
	BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC SampleDesc;
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc = BufferDesc;
	SwapChainDesc.SampleDesc = SampleDesc;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = WindowHwnd;
	SwapChainDesc.Windowed = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &FeatureLevel, &Context) < 0)
	{
		DeleteWindow();
		return false;
	}

	MethodsTable = (uint64_t*)::calloc(205, sizeof(uint64_t));
	memcpy(MethodsTable, *(uint64_t**)SwapChain, 18 * sizeof(uint64_t));
	memcpy(MethodsTable + 18, *(uint64_t**)Device, 43 * sizeof(uint64_t));
	memcpy(MethodsTable + 18 + 43, *(uint64_t**)Context, 144 * sizeof(uint64_t));
	Sleep(1000);

	SwapChain->Release();
	SwapChain = 0;
	Device->Release();
	Device = 0;
	Context->Release();
	Context = 0;
	DeleteWindow();
	return true;
}

bool DX11Window::HookD3D()
{
	if (GetDX11Context())
	{
		Hooking::CreateHook((void*)MethodsTable[8], &PresentHook, (void**)&Present);
		Hooking::CreateHook((void*)MethodsTable[13], &ResizeHook, (void**)&ResizeBuffers);
		return true;
	}
	return false;
}

void DX11Window::UnhookD3D()
{
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
	Hooking::DisableHook((void*)MethodsTable[8]);
	Hooking::DisableHook((void*)MethodsTable[13]);
	free(MethodsTable);
}

void Initialize(HMODULE dwModule)
{
	UNREFERENCED_PARAMETER(dwModule);

	g_Engine = std::make_unique<Engine>();
	g_Console->Initialize(skCrypt("Debug Console"), true);
	g_DX11Window->HookD3D();
	g_Game->Initilize();
	g_Hooking->Initialize();
}