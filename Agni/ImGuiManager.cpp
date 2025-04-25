#include "ImGuiManager.h"
#include "ImGui/imgui.h"
ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
