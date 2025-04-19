#include "App.h"
#include <iomanip>
#include <sstream>
#include "AgniTimer.h"
#include "Bindable.h"
#include "VertexShader.h"
#include "Box.h"
App::App() :
	window(1280, 720, L"Agni Engine")
{
	window.Gfx().ClearBuffer(0, 0, 0);
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
	this->box = std::make_unique<Box>(window.Gfx());
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
	const float dt = timer.Mark();
	window.Gfx().ClearBuffer(0, 0, 0);
	box->Update(dt);
	box->Draw(window.Gfx());

	window.Gfx().EndFrame();
} 