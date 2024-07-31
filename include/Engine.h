#pragma once

#define MAX_VIRTUALKEYS 0x100

struct VirtualKey
{
	bool bKey, bDown, bUp;
};
inline VirtualKey VirtualKeys[MAX_VIRTUALKEYS];

class Engine
{
public:
	bool GetKeyPress(int VKey, bool Immediate);

	Engine();
	~Engine();
};
inline std::unique_ptr<Engine> g_Engine;