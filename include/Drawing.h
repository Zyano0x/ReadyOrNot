#pragma once

enum BarType
{
	Health,
	Shield
};

class Draw
{
public:
	explicit Draw();
	~Draw() noexcept;

	static void DrawLine(float x1, float y1, float x2, float y2, float thickness, ImVec4 color);
	static void DrawCircle(float x1, float y1, float radius, ImVec4 color);
	static void DrawCircleFilled(float x1, float y1, float radius, ImVec4 color);
	static void DrawRect(float x, float y, float w, float h, ImVec4 color, float rounding);
	static void DrawRectFilled(float x, float y, float w, float h, ImVec4 color, float rounding);
	static void DrawTriangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, ImVec4 color);
	static void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled, ImVec4 color, float thickness);
	static void DrawBox(float x, float y, float w, float h, bool filled, ImVec4 color);
	static void DrawBoxOutline(float x, float y, float w, float h, bool filled, ImVec4 color);
	static void DrawCornersBox(float x, float y, float w, float h, bool filled, ImVec4 color);
	static void DrawCornersBoxOutline(float x, float y, float w, float h, bool filled, ImVec4 color);
	static void DrawString(ImFont* font, std::string text, float x, float y, float size, bool shadow, ImVec4 color);
	static void VerticalHealthBar(int x, int y, int w, int h, int Health, bool Outlined, BarType BarType);
	static void HorizontalHealthBar(int x, int y, int w, int h, int Value, bool Outlined, BarType barType);
};
