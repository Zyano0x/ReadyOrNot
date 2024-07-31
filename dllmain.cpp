#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Initialize, hModule, 0, 0);
		if (hThread) CloseHandle(hThread);
	}
	return TRUE;
}

