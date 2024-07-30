#pragma once

class Menu
{
public:
	bool m_IsShowMenu{ false };

public:
	void Draw();
	void MainMenu();

	Menu() = default;
	~Menu() = default;
} extern g_Menu;