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
	window.Gfx().ClearBuffer(0.5f, c, c);
	window.Gfx().EndFrame();
} 