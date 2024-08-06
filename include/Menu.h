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
};
inline std::unique_ptr<Menu> g_Menu;