#include "App.h"
#include <iomanip>
#include <sstream>
#include "AgniTimer.h"
#include "Bindable.h"
#include "VertexShader.h"
#include <ctime>
#include <random>
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

App::App() :
	window(1280, 720, L"Agni Engine"),
	light(window.Gfx())
{
	
	cube = std::make_unique<Cube>(window.Gfx(), 8.0f, 0.0f, 0.0f, 0.0f,0.0f);
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
	
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
	cube->Update(dt);
	cube->Draw(gfx);
	if (ImGui::Begin("Agni Editor")) {
		ImGui::Text("Frame Rate: %.1f", ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Speed", &speed_factor, 0.0f, 10.0f, "%.1f");
	}
	light.SpawnControlWindow();
	cam.SpawnCameraController();
	ImGui::End();
	window.Gfx().EndFrame();
} 