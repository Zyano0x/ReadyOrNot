#include "pch.h"

Menu g_Menu;

void Menu::Draw()
{
	if (g_Menu.m_IsShowMenu)
	{
		g_Menu.MainMenu();
	}
}

void Menu::MainMenu()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& Style = ImGui::GetStyle();

	Style.FrameBorderSize = 1;
	Style.WindowBorderSize = 1;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, Settings[WINDOW_BG].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, Settings[WINDOW_BG].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, Settings[CHILD_BG].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_Text, Settings[TEXT].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_TextHovered, Settings[TEXT_HOVERED].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_TextActive, Settings[TEXT_ACTIVE].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, Settings[FRAME_BG].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Settings[FRAME_BG_HOVERED].Value.v4Value);
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Settings[FRAME_BG_ACTIVE].Value.v4Value);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 495));

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(500, 495), ImGuiCond_Once);
	if (!ImGui::Begin(skCrypt("Nixware"), NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		return;

	ImGui::BeginTabBar("Tabs");
	{
		if (ImGui::BeginTabItem("Aimbot"))
		{
			ImVec2 ChildSize = ImVec2(ImGui::GetColumnWidth(), ImGui::GetWindowHeight() - (ImGui::GetCursorPosY() + Style.ItemInnerSpacing.y * 2));

			ImGui::BeginChild("Globals", ChildSize);
			{
				ImGui::Checkbox("Enabled", &Settings[AIM_ENABLED].Value.bValue);

				if (!Settings[AIM_ENABLED].Value.bValue)
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				{
					ImGui::Checkbox("Visible Check", &Settings[VISIBLE_CHECK].Value.bValue);
					ImGui::Checkbox("Ignore Surrendered", &Settings[IGNORE_SURRENDERED].Value.bValue);
					ImGui::Combo("Mode", &Settings[AIM_MODE].Value.iValue, "Normal\0" "Silent\0");
					ImGui::Combo("Hitbox", &Settings[AIM_BONE].Value.iValue, "Head\0" "Body\0");
					ImGui::SliderFloat("Smooth", &Settings[AIM_SMOOTH].Value.fValue, Settings[AIM_SMOOTH].Value.fMin, Settings[AIM_SMOOTH].Value.fMax, "%.1f", ImGuiSliderFlags_NoInput);

					ImGui::Spacing();

					ImGui::Checkbox("FOV", &Settings[FOV].Value.bValue); ImGui::ColorEdit4("FOV Color", (float*)&Settings[FOV_COLOR].Value.v4Value, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
					ImGui::SliderFloat("FOV Radius", &Settings[FOV_RADIUS].Value.fValue, Settings[FOV_RADIUS].Value.fMin, Settings[FOV_RADIUS].Value.fMax, "%.1f", ImGuiSliderFlags_NoInput);
				}
				if (!Settings[AIM_ENABLED].Value.bValue)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}
			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Visuals"))
		{
			ImVec2 ChildSize = ImVec2(ImGui::GetColumnWidth(), ImGui::GetWindowHeight() - (ImGui::GetCursorPosY() + Style.ItemInnerSpacing.y * 2));

			ImGui::BeginChild("Player ESP", ChildSize);
			{
				ImGui::Checkbox("Enabled", &Settings[ESP_ENABLED].Value.bValue);

				if (!Settings[ESP_ENABLED].Value.bValue)
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				{
					ImGui::Checkbox("Enemy", &Settings[ESP_ENEMY].Value.bValue); ImGui::ColorEdit4("Enemy Color", (float*)&Settings[ESP_ENEMY_COLOR].Value.v4Value, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
					ImGui::Checkbox("Team", &Settings[ESP_FRIENDLY].Value.bValue); ImGui::ColorEdit4("Team Color", (float*)&Settings[ESP_FRIENDLY_COLOR].Value.v4Value, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
					ImGui::Checkbox("Civilian", &Settings[ESP_CIVILIAN].Value.bValue); ImGui::ColorEdit4("Civilian Color", (float*)&Settings[ESP_CIVILIAN_COLOR].Value.v4Value, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
					ImGui::Checkbox("Name", &Settings[ESP_NAME].Value.bValue);
					ImGui::Checkbox("Skeleton", &Settings[ESP_SKELETON].Value.bValue);
					ImGui::Checkbox("Snaplines", &Settings[ESP_SNAP_LINES].Value.bValue);
					ImGui::Checkbox("Distance", &Settings[ESP_DISTANCE].Value.bValue);
					ImGui::Checkbox("Health Bar", &Settings[ESP_HEALTH].Value.bValue);
					ImGui::Checkbox("Weapon", &Settings[ESP_WEAPON].Value.bValue);
					ImGui::Checkbox("Evidence", &Settings[ESP_EVIDENCE].Value.bValue); ImGui::ColorEdit4("Evidence Color", (float*)&Settings[ESP_EVIDENCE_COLOR].Value.v4Value, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
					ImGui::Checkbox("Trap", &Settings[ESP_TRAP].Value.bValue); ImGui::ColorEdit4("Trap Color", (float*)&Settings[ESP_TRAP_COLOR].Value.v4Value, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoDragDrop);
					ImGui::Combo("Box", &Settings[ESP_BOX].Value.iValue, "Off\0" "2D Filled\0" "Corner Filled\0");
				}
				if (!Settings[ESP_ENABLED].Value.bValue)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}
			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Miscellaneous"))
		{
			ImVec2 ChildSize = ImVec2(ImGui::GetColumnWidth(), ImGui::GetWindowHeight() - (ImGui::GetCursorPosY() + Style.ItemInnerSpacing.y * 2));

			ImGui::BeginChild("Player", ChildSize);
			{
				ImGui::Checkbox("No Recoil", &Settings[NO_RECOIL].Value.bValue);
				ImGui::Checkbox("Infinite Ammo", &Settings[INFINITE_AMMO].Value.bValue);
				ImGui::Checkbox("Break All Doors", &Settings[BREAK_DOOR].Value.bValue);
				ImGui::Checkbox("Disarm All Traps", &Settings[DISARM_TRAP].Value.bValue);
				ImGui::Checkbox("Speed", &Settings[SPEED].Value.bValue);
				ImGui::Checkbox("FOV Changer", &Settings[FOV_CHANGER].Value.bValue);
				ImGui::Checkbox("Free Cam", &Settings[FREE_CAM].Value.bValue);
				ImGui::Checkbox("Auto Arrest", &Settings[AUTO_ARREST].Value.bValue);
				ImGui::Checkbox("Auto Report", &Settings[AUTO_REPORT].Value.bValue);
				ImGui::Checkbox("Auto Secure Evidence", &Settings[AUTO_SECURE_EVIDENCE].Value.bValue);

				ImGui::SliderInt("Speed Multiplier", &Settings[SPEED_MULTIPLIER].Value.iValue, Settings[SPEED_MULTIPLIER].Value.iMin, Settings[SPEED_MULTIPLIER].Value.iMax);
				ImGui::SliderFloat("FOV", &Settings[FOV_AMOUNT].Value.fValue, Settings[FOV_AMOUNT].Value.fMin, Settings[FOV_AMOUNT].Value.fMax);
			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Settings"))
		{
			ImVec2 ChildSize = ImVec2((ImGui::GetColumnWidth() - (Style.ItemSpacing.x)) / 2, ImGui::GetWindowHeight() - (ImGui::GetCursorPosY() + Style.ItemInnerSpacing.y * 2));

			ImGui::BeginChild("Info", ChildSize);
			{
				ImGui::LabelText("Last update:", __DATE__);
				ImGui::LabelText("Developer:", "Zyano0x");
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("Configs", ChildSize);
			{
				float ColumnWidth = ImGui::GetColumnWidth();

				if (ImGui::Button("Load Configs", ImVec2(ColumnWidth - 10.f, 35.f)))
				{
					if (BotLoadSettings())
					{
						ImGui::OpenPopup("Settings Loaded");
					}
					else
					{
						ImGui::OpenPopup("Loading Failed");
					}
				}

				if (ImGui::Button("Save Configs", ImVec2(ColumnWidth - 10.f, 35.f)))
				{
					if (BotSaveSettings())
					{
						ImGui::OpenPopup("Settings Saved");
					}
					else
					{
						ImGui::OpenPopup("Saving Failed");
					}
				}

				if (ImGui::BeginPopupModal("Settings Loaded", 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("The settings have been loaded");
					ImGui::Separator();
					if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Settings Saved", 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("The settings have been saved");
					ImGui::Separator();
					if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Loading Failed", 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Failed to load the settings");
					ImGui::Separator();
					if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopupModal("Saving Failed", 0, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Failed to save the settings");
					ImGui::Separator();
					if (ImGui::Button("OK", ImVec2(70, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	ImGui::End();

	ImGui::PopStyleColor(9);
	ImGui::PopStyleVar();
}
