#include "App.h"
#include <iomanip>
#include <sstream>
#include "AgniTimer.h"
#include "Bindable.h"
#include "VertexShader.h"
#include <ctime>
#include <random>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

App::App() :
	window(1280, 720, L"Agni Engine"),
	light(window.Gfx())
{
	int count = 400;
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 100.0f);
	std::uniform_real_distribution<float> cdist(0.0f, 1.0f);
	
	for (auto i = 0; i < count; i++)
	{
		DirectX::XMFLOAT3 materialColor = { cdist(rng), cdist(rng), cdist(rng)};
		boxes.push_back(std::make_unique<Box>(
			window.Gfx(), rng, adist,
			ddist, odist, rdist,
			materialColor
		));
	}

	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 500.0f));
	
}

int App::Go() 
{
	while (true) {
		if (const auto ecode = Window::ProcessMessage()) {
			return *ecode;
		}
		DoFrame();
	}


}

void App::DoFrame()
{
	window.Gfx().BeginFrame(0, 0, 0);
	Graphics& gfx = window.Gfx();
	window.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(gfx);
	const float dt = timer.Mark()*speed_factor;
	for (auto & drawable : boxes) {
		drawable->Update(dt);
		drawable->Draw(gfx);
	}
	bool opened = true;
	ImGui::SetNextWindowSize(ImVec2(1280 / 6, 720 / 8));
	ImGui::SetNextWindowPos(ImVec2(1280 - (1280 / 6),0.0f));
	

	if (ImGui::Begin("Agni Editor", &opened, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse)) {
		ImGui::Text("Mouse X: %i, Mouse Y: %i", window.mouse.GetPosX(), window.mouse.GetPosY());
		ImGui::Text("Mouse X: %i, Mouse Y: %i", (int)ImGui::GetIO().MousePos.x, (int)ImGui::GetIO().MousePos.y);
		ImGui::SliderFloat("Speed", &speed_factor, 0.0f, 10.0f, "%.1f");
	}
	//light
	light.SpawnControlWindow();
	light.Draw(gfx);
	//camera
	cam.SpawnCameraController();
	cam.Update(0);
	//present
	ImGui::End();
	window.Gfx().EndFrame();
} 