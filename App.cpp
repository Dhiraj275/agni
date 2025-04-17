#include "App.h"
#include <iomanip>
#include <sstream>
#include "AgniTimer.h"
App::App() :
	window(1280, 720, L"Agni Engine")
{

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
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	window.Gfx().ClearBuffer(0, 0, 0);

	window.Gfx().DrawTestTriangle(timer.Peek(),		-2.0f,		0.0f);
	window.Gfx().DrawTestTriangle(timer.Peek(),			0.0f,		0.0f);
	window.Gfx().DrawTestTriangle(timer.Peek(),		2.0f,		0.0f);

	window.Gfx().DrawTestTriangle(timer.Peek(),		-2.0f,			-2.0f);
	window.Gfx().DrawTestTriangle(timer.Peek(),	0.0f,			-2.0f);
	window.Gfx().DrawTestTriangle(timer.Peek(),		2.0f,			-2.0f);

	window.Gfx().DrawTestTriangle(timer.Peek(),		-2.0f,			2.0f);
	window.Gfx().DrawTestTriangle(timer.Peek(),	0.0f,			2.0f);
	window.Gfx().DrawTestTriangle(timer.Peek(),		2.0f,			2.0f);


	window.Gfx().EndFrame();
} 