#pragma once

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DX11Window
{
public:
	void Overlay(IDXGISwapChain* SwapChain);

public:
	explicit DX11Window();
	~DX11Window() noexcept;

private:
	HWND hWnd = nullptr;
	WNDPROC WndProc = nullptr;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	bool InitImGui = false;

	typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags);
	IDXGISwapChainPresent Present = nullptr;

	typedef HRESULT(APIENTRY* IDXGISwapChainResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	IDXGISwapChainResizeBuffers ResizeBuffers = nullptr;

public:
	static void Initialize(HMODULE dwModule);

	static LRESULT APIENTRY	WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY	PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static HRESULT APIENTRY ResizeHook(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
} extern g_DX11Window;