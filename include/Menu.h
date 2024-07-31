#pragma once

class Menu
{
public:
	static inline bool m_IsShowMenu{ false };

public:
	static void Draw();
	static void MainMenu();

	Menu() = default;
	~Menu() = default;
};
