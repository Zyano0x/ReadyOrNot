#pragma once

class Render
{
public:
	bool m_IsShowMenu{ false };

public:
	void Menu();
	void MainMenu();
	void Radar();

	Render() = default;
	~Render() = default;
};
inline std::unique_ptr<Render> g_Render = std::make_unique<Render>();