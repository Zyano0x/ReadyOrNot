#include "pch.h"

Engine g_Engine;

bool Engine::GetKeyPress(int VKey, bool Immediate)
{
	if (VirtualKeys[VKey].bKey)
	{
		VirtualKeys[VKey].bUp = false;
		VirtualKeys[VKey].bDown = true;
	}

	else if (!VirtualKeys[VKey].bKey && VirtualKeys[VKey].bDown)
	{
		VirtualKeys[VKey].bUp = true;
		VirtualKeys[VKey].bDown = false;
	}

	else
	{
		VirtualKeys[VKey].bUp = false;
		VirtualKeys[VKey].bDown = false;
	}

	if (Immediate)
		return VirtualKeys[VKey].bDown;

	else
		return VirtualKeys[VKey].bUp;
}
