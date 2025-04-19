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
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
	int count = 5;
	std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_real_distribution<float> xDist(-180.0f, 180.0f);     // Random x position
	std::uniform_real_distribution<float> yDist(-180.0f, 180.0f);     // Random y position
	std::uniform_real_distribution<float> zDist(-60.0f, 0.0f);      // Random z position
	std::uniform_real_distribution<float> speedDist(6.5f, 200.0f);    // Random speed
	std::uniform_real_distribution<float> angleDist(0.0f, 6.28f);   // Random angle (0 to 2π)

	// Create the specified number of boxes
	for (int i = 0; i < count; i++) {
		float x = xDist(rng);
		float y = yDist(rng);
		float z = zDist(rng);
		float speed = speedDist(rng);
		float angle = angleDist(rng);

		// Create a box with random parameters and add it to the vector
		boxes.push_back(std::make_unique<Box>(window.Gfx(), x, y, z, speed, angle));
	}
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
	for (auto& box : boxes)
	{
		box->Update(dt);
		box->Draw(window.Gfx());
	}
	window.Gfx().EndFrame();
} 