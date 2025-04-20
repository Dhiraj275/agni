#include "App.h"
#include <iomanip>
#include <sstream>
#include "AgniTimer.h"
#include "Bindable.h"
#include "VertexShader.h"
#include "Box.h"
#include <ctime>
#include <random>
App::App() :
	window(1280, 720, L"Agni Engine")
{
	window.Gfx().ClearBuffer(0, 0, 0);
	sphere = std::make_unique<Sphere>(window.Gfx(),0.0f,0.0f,0.0f,5.0f,0.0f,0.0f);
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
	Graphics& gfx = window.Gfx();
	const float dt = timer.Mark();
	window.Gfx().ClearBuffer(0, 0, 0);
	sphere->Update(dt);
	sphere->Draw(gfx);
	window.Gfx().EndFrame();
} 