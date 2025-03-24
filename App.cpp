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
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	window.SetTitle(oss.str());
} 