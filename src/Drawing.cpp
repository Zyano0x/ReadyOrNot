#include "pch.h"

Draw::Draw() {}

Draw::~Draw() {}

void Draw::DrawLine(float x1, float y1, float x2, float y2, float thickness, ImVec4 color)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32(color), thickness);
}

void Draw::DrawCircle(float x1, float y1, float radius, ImVec4 color)
{
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x1, y1), radius, ImGui::GetColorU32(color));
}

void Draw::DrawCircleFilled(float x1, float y1, float radius, ImVec4 color)
{
	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x1, y1), radius, ImGui::GetColorU32(color));
}

void Draw::DrawRect(float x, float y, float w, float h, ImVec4 color, float rounding)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(color), rounding);
}

void Draw::DrawRectFilled(float x, float y, float w, float h, ImVec4 color, float rounding)
{
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(color), rounding);
}

void Draw::DrawTriangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, ImVec4 color)
{
	ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::GetColorU32(color));
}

void Draw::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled, ImVec4 color, float thickness)
{
	if (filled)
		ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.5f)));

	ImGui::GetBackgroundDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), ImGui::GetColorU32(color), thickness);
}

void Draw::DrawBox(float x, float y, float w, float h, bool filled, ImVec4 color)
{
	if (filled)
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(ImVec4(color.x, color.y, color.z, 0.2f)));

	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(color));
}

void Draw::DrawBoxOutline(float x, float y, float w, float h, bool filled, ImVec4 color)
{
	DrawBox(x + 1.f, y + 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawBox(x - 1.f, y - 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawBox(x - 1.f, y + 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawBox(x + 1.f, y - 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));

	DrawBox(x + 1.f, y, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawBox(x - 1.f, y, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawBox(x, y + 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawBox(x, y - 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));

	DrawBox(x, y, w, h, filled, color);
}

void Draw::DrawCornersBox(float x, float y, float w, float h, bool filled, ImVec4 color)
{
	if (filled)
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w + 1.0f, y + h + 1.0f), ImGui::GetColorU32(ImVec4(color.x, color.y, color.z, 0.2f)));

	DrawLine(x, y, x + w / 4.0f, y, 1.5f, color);
	DrawLine(x, y, x, y + h / 4.0f, 1.5f, color);

	DrawLine(x + w, y, x + w - w / 4.0f, y, 1.5f, color);
	DrawLine(x + w, y, x + w, y + h / 4.0f, 1.5f, color);

	DrawLine(x, y + h, x + w / 4.0f, y + h, 1.5f, color);
	DrawLine(x, y + h, x, y + h - h / 4.0f, 1.5f, color);

	DrawLine(x + w, y + h, x + w, y + h - h / 4.0f, 1.5f, color);
	DrawLine(x + w, y + h, x + w - w / 4.0f, y + h, 1.5f, color);
}

void Draw::DrawCornersBoxOutline(float x, float y, float w, float h, bool filled, ImVec4 color)
{
	DrawCornersBox(x - 1.f, y + 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawCornersBox(x - 1.f, y - 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawCornersBox(x + 1.f, y + 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawCornersBox(x + 1.f, y - 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));

	DrawCornersBox(x - 1.f, y, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawCornersBox(x, y - 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawCornersBox(x, y + 1.f, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));
	DrawCornersBox(x + 1.f, y, w, h, false, ImVec4(0.0f, 0.0f, 0.0f, color.w));

	DrawCornersBox(x, y, w, h, filled, color);
}

void Draw::DrawString(ImFont* font, std::string text, float x, float y, float size, bool shadow, ImVec4 color)
{
	float textWidth = ImGui::CalcTextSize(text.c_str()).x;

	if (shadow)
	{
		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2 - 1.0f, y - 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2, y - 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());

		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2 + 1.0f, y - 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2 + 1.0f, y), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());

		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2 + 1.0f, y + 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2, y + 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());

		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2 - 1.0f, y + 1.0f), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2 - 1.0f, y), ImGui::GetColorU32(ImGui::GetColorU32(ImVec4(0.000000f, 0.000000f, 0.000000f, 1.000000f))), text.c_str());
	}

	ImGui::GetBackgroundDrawList()->AddText(font, size, ImVec2(x - textWidth / 2, y), ImGui::GetColorU32(color), text.c_str());
}

void Draw::VerticalHealthBar(int x, int y, int w, int h, int Value, bool Outlined, BarType barType)
{
	auto vList = ImGui::GetBackgroundDrawList();

	int clampedValue = max(0, min(Value, 100));

	ImColor barColor;
	if (barType == Health) {
		// Health bar color calculation (green to red)
		barColor = ImColor(
			min(510 * (100 - clampedValue) / 100, 255), // Red increases as health decreases
			min(510 * clampedValue / 100, 255),         // Green decreases as health decreases
			25,
			255
		);
	}
	else if (barType == Shield) {
		// Shield bar color calculation (blue to gray)
		barColor = ImColor(
			min(173 * (100 - clampedValue) / 100 + 211 * clampedValue / 100, 255), // Red component
			min(216 * (100 - clampedValue) / 100 + 211 * clampedValue / 100, 255), // Green component
			min(240 * (100 - clampedValue) / 100 + 211 * clampedValue / 100, 255), // Blue component
			255
		);
	}

	if (Outlined)
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

	vList->AddRectFilled(ImVec2(x, y + (h - static_cast<int>(((float)h / 100.0f) * (float)clampedValue))), ImVec2(x + w, y + h), barColor, 0.0f, 0);
}

void Draw::HorizontalHealthBar(int x, int y, int w, int h, int Value, bool Outlined, BarType barType)
{
	auto vList = ImGui::GetBackgroundDrawList();

	int clampedValue = max(0, min(Value, 100));

	ImColor barColor;
	if (barType == Health) {
		// Health bar color calculation (green to red)
		barColor = ImColor(
			min(510 * (100 - clampedValue) / 100, 255), // Red increases as health decreases
			min(510 * clampedValue / 100, 255),         // Green increases as health decreases
			25,
			255
		);
	}
	else if (barType == Shield) {
		// Shield bar color calculation (blue to gray)
		barColor = ImColor(
			min(173 * (100 - clampedValue) / 100 + 211 * clampedValue / 100, 255), // Red component
			min(216 * (100 - clampedValue) / 100 + 211 * clampedValue / 100, 255), // Green component
			min(240 * (100 - clampedValue) / 100 + 211 * clampedValue / 100, 255), // Blue component
			255
		);
	}

	if (Outlined)
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

	vList->AddRectFilled(ImVec2(x, y), ImVec2(x + static_cast<int>(((float)w / 100.0f) * (float)clampedValue), y + h), barColor, 0.0f, 0);
}