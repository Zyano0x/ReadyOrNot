#pragma once

#define EXT(f) throw std::exception(#f) 
#define EXI(p, c, f, ...) p = f(__VA_ARGS__); if (p == c) { EXT(f); } 

#define SC(t, v) static_cast<t>(v)
#define RC(t, v) reinterpret_cast<t>(v)

#define FUNCTION_INDEX(i) SC(uint16_t, i)
#define FUNCTION_HOOK(f) RC(uint64_t, &f)

#define ORIGINAL(p, f, t, i) static auto p = RC(decltype(&f), t->getOriginals().at(i))

#define VTSWAP(p, a, r) if(a) { EXI(p, nullptr, std::make_unique<PLH::VTableSwapHook>, RC(const uint64_t, a), r) if(p) { p->hook(); } }

#define UNHOOK(p) if(p) { p->unHook(); }

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DX11Window
{
public:
	void Overlay(IDXGISwapChain* SwapChain);

public:
	explicit DX11Window();
	~DX11Window() noexcept;

	bool HookD3D();
	void UnhookD3D();
	bool InitWindow();
	bool DeleteWindow();

private:
	PLH::VFuncMap m_VTOrig{};
	std::unique_ptr<PLH::VFuncSwapHook> VFHook = nullptr;

	WNDCLASSEX WindowClass{};
	HWND WindowHwnd = nullptr;
	HWND hWnd = nullptr;
	WNDPROC WndProc = nullptr;
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	bool InitImGui = false;
	bool Unload = false;

	typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags);
	uintptr_t Present = 0;

	typedef HRESULT(APIENTRY* IDXGISwapChainResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	IDXGISwapChainResizeBuffers ResizeBuffers = nullptr;

public:
	static void Initialize(HMODULE dwModule);

	static LRESULT APIENTRY	WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY	PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static HRESULT APIENTRY ResizeHook(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
};
inline std::unique_ptr<DX11Window> g_DX11Window;