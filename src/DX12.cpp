#include "pch.h"

static LRESULT WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	if (g_Engine.GetKeyPress(VK_INSERT, false))
		g_Menu.m_IsShowMenu = !g_Menu.m_IsShowMenu;

	if (g_Menu.m_IsShowMenu)
	{
		ImGui_ImplWin32_WndProcHandler((HWND)WndProc, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc((WNDPROC)WndProc, hWnd, uMsg, wParam, lParam);
}

static HRESULT PresentHook(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (!p_Device)
	{
		if (SUCCEEDED(SwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&p_Device)))
		{
			p_Device->GetImmediateContext(&p_DeviceContext);
			DXGI_SWAP_CHAIN_DESC SwapChainDesc;
			SwapChain->GetDesc(&SwapChainDesc);
			hWnd = SwapChainDesc.OutputWindow;

			ID3D11Texture2D* Buffer;
			SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&Buffer);
			p_Device->CreateRenderTargetView(Buffer, NULL, &p_RenderTarget);
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
			ImGui_ImplDX11_Init(p_Device, p_DeviceContext);
			ImGui_ImplDX11_CreateDeviceObjects();

			if (strcpy_s(szDllDirectory, sizeof(szDllDirectory), "C:\\RC\\") == 0)
				CreateDirectoryA(szDllDirectory, NULL);

			BotInitSettings();
			BotLoadSettings();
		}
	}

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = g_Menu.m_IsShowMenu;

	spoof_call(Game::Setup);
	spoof_call(Game::Visual);
	spoof_call(Game::Misc);

	g_Menu.Draw();

	ImGui::EndFrame();
	ImGui::Render();
	p_DeviceContext->OMSetRenderTargets(1, &p_RenderTarget, NULL);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData());

	return oPresent(SwapChain, SyncInterval, Flags);
}

static HRESULT ExecuteCommandListsHook(ID3D12CommandQueue* Queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists)
{

}

static HRESULT ResizeHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	if (p_RenderTarget)
	{
		p_DeviceContext->OMSetRenderTargets(0, 0, 0);
		p_RenderTarget->Release();
	}

	HRESULT hr = oResizeBuffers(SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* Buffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&Buffer);
	p_Device->CreateRenderTargetView(Buffer, NULL, &p_RenderTarget);

	Buffer->Release();

	p_DeviceContext->OMSetRenderTargets(1, &p_RenderTarget, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	p_DeviceContext->RSSetViewports(1, &vp);

	return hr;
}

VOID Main()
{
	g_Console.Initialize(skCrypt("Debug Console"), true);

	uint64_t hkPresent_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();
	uint64_t hkExecuteCommandLists_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 4D 8B F0 48 8B FA"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();
	uint64_t hkResizeBuffers_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 8B FA"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();
	uint64_t CreateHook_Sig = Signature(std::string(skCrypt("48 89 5C 24 ? 57 48 83 EC ? 33 C0 48 89 44 24"))).Import(std::string(skCrypt("GameOverlayRenderer64.dll"))).GetPointer();

	__int64(__fastcall * CreateHook)(unsigned __int64 pFuncAddress, __int64 pDetourFuncAddress, unsigned __int64* pOriginalFuncAddressOut, int a4);
	CreateHook = (decltype(CreateHook))CreateHook_Sig;
	CreateHook(hkPresent_Sig, (__int64)&PresentHook, (unsigned __int64*)&oPresent, 1);
	CreateHook(hkExecuteCommandLists_Sig, (__int64)&ExecuteCommandListsHook, (unsigned __int64*)&oExecuteCommandLists, 1);
	CreateHook(hkResizeBuffers_Sig, (__int64)&ResizeHook, (unsigned __int64*)&oResizeBuffers, 1);

	/*IDXGISwapChain* SwapChain = nullptr;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* Context = nullptr;
	auto FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = { 0 };
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.OutputWindow = GetForegroundWindow();
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.Windowed = TRUE;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, nullptr, &Context))) {
		return;
	}

	auto VTable = *reinterpret_cast<PVOID**>(SwapChain);
	auto Present = VTable[8];
	auto Resize = VTable[13];

	Context->Release();
	Device->Release();
	SwapChain->Release();

	MH_Initialize();

	MH_CreateHook(Present, &PresentHook, reinterpret_cast<PVOID*>(&oPresent));
	MH_EnableHook(Present);

	MH_CreateHook(Resize, &ResizeHook, reinterpret_cast<PVOID*>(&oResizeBuffers));
	MH_EnableHook(Resize);*/
}