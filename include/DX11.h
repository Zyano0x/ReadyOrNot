#pragma once

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline uint64_t PresentFn = 0;
inline uint64_t ResizeBuffersFn = 0;

class DX11Window
{
public:
	void Overlay(IDXGISwapChain* SwapChain);

public:
	explicit DX11Window();
	~DX11Window() noexcept;

	void HookD3D();
	void UnhookD3D();

private:
	WNDCLASSEX WindowClass{};
	HWND WindowHwnd = nullptr;
	HWND hWnd = nullptr;
	WNDPROC WndProc = nullptr;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	bool InitImGui = false;

public:
	typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags);
	IDXGISwapChainPresent oPresent = 0;

	typedef HRESULT(APIENTRY* IDXGISwapChainResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	IDXGISwapChainResizeBuffers	oResizeBuffers = 0;

	static LRESULT APIENTRY	WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY	PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static HRESULT APIENTRY ResizeHook(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
};
inline std::unique_ptr<DX11Window> g_DX11Window = std::make_unique<DX11Window>();

void Initialize();
void Deallocate();