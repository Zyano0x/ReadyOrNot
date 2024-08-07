#pragma once

class Hooking
{
public:
	void Initialize();
	void Shutdown();
	static bool CreateHook(LPVOID Target, LPVOID Detour, LPVOID* Orig);
	static void	EnableHook(LPVOID Target);
	static void	EnableAllHooks();
	static void	DisableHook(LPVOID Target);
	static void	RemoveHook(LPVOID Target);
	static void	DisableAllHooks();
	static void	RemoveAllHooks();

public:
	explicit Hooking();
	~Hooking() noexcept;
};
inline std::unique_ptr<Hooking> g_Hooking = std::make_unique<Hooking>();