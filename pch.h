// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <vector>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <functional>

#include <dxgi.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "MinHook/MinHook.h"
#pragma comment(lib, "MinHook.lib")

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_impl_win32.h"

#include "Font/Fonts.h"

#include "Lazy.h"
#include "Obfuscate.h"
#include "Memory.h"
#include "Spoofer.h"

#include "SDK.h"
#include "Hooking.h"
#include "Console.h"
#include "CVars.h"
#include "Engine.h"
#include "Menu.h"
#include "Drawing.h"
#include "Feature.h"
#include "DX12.h"

#endif //PCH_H
