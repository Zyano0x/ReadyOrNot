#pragma once

inline HWND hWnd = nullptr;
inline WNDPROC WndProc = nullptr;
inline ID3D12Device* p_Device = nullptr;
inline ID3D12CommandQueue* CommandQueue = nullptr;
inline ID3D12GraphicsCommandList* CommandList = nullptr;
inline ID3D12DescriptorHeap* DescriptorHeapBackBuffers = nullptr;
inline ID3D12DescriptorHeap* DescriptorHeapImGuiRender = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(WINAPI* tPresent)(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags);
inline tPresent oPresent = nullptr;

typedef void(WINAPI* ExecuteCommandLists)(ID3D12CommandQueue* Queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);
ExecuteCommandLists oExecuteCommandLists = nullptr;

typedef HRESULT(WINAPI* tResizeBuffers)(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
inline tResizeBuffers oResizeBuffers = nullptr;

VOID Main();