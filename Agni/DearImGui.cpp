#include "DearImGui.h"
#include "ImGui/imgui.h"
DearImGui::DearImGui()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	style.WindowRounding = 5.0f;
	style.FrameRounding = 5.0f;
	style.ScrollbarRounding = 5.0f;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 0.85f, 0.70f, 1.00f); // warm beige
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.05f, 0.00f, 1.00f); // dark ember
	colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.06f, 0.00f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.07f, 0.00f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.25f, 0.00f, 0.30f);

	colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.10f, 0.00f, 0.50f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.45f, 0.00f, 0.50f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.50f, 0.00f, 0.80f);

	colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.10f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.60f, 0.20f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.05f, 0.00f, 0.75f);

	colors[ImGuiCol_Button] = ImVec4(0.75f, 0.30f, 0.00f, 0.80f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.45f, 0.00f, 0.85f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.60f, 0.10f, 1.00f);

	colors[ImGuiCol_Header] = ImVec4(0.75f, 0.30f, 0.00f, 0.80f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.45f, 0.00f, 0.85f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.60f, 0.10f, 1.00f);

	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.50f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.60f, 0.10f, 1.00f);

	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.40f, 0.00f, 0.70f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.60f, 0.10f, 0.90f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.70f, 0.20f, 1.00f);

	colors[ImGuiCol_Separator] = ImVec4(0.75f, 0.30f, 0.00f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.40f, 0.00f, 0.80f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.60f, 0.10f, 1.00f);
}
