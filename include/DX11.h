#pragma once

#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DX11Window
{
public:
	bool GetDX11Context();
	bool HookD3D();
	void UnhookD3D();
	bool InitWindow();
	bool DeleteWindow();
	void Overlay(IDXGISwapChain* SwapChain);

public:
	explicit DX11Window();
	~DX11Window() noexcept;

private:
	WNDCLASSEX WindowClass{};
	HWND WindowHwnd = nullptr;
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

private:
	static LRESULT APIENTRY	WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY	PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static HRESULT APIENTRY ResizeHook(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
};
inline std::unique_ptr<DX11Window> g_DX11Window;

void Initialize(HMODULE dwModule);