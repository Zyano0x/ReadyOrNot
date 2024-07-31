#include "pch.h"

Engine::Engine()
{
	g_Console = std::make_unique<Console>();
	g_DX11Window = std::make_unique<DX11Window>();
	g_Hooking = std::make_unique<Hooking>();
	g_Game = std::make_unique<Game>();
}

Engine::~Engine()
{
	g_Game.release();
	g_Hooking.release();
	g_DX11Window.release();
	g_Console.release();
}

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