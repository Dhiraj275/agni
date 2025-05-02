#pragma once
#include "Window.h"
#include "AgniTimer.h"
#include "Sphere.h"
#include "Camera.h"
#include "Box.h"
#include "Cube.h"
#include "PointLight.h"
class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window window;
	Camera cam;
	AgniTimer timer;
	PointLight light;
	std::vector<std::unique_ptr<class Box >> boxes;
	std::unique_ptr<class Sphere > sphere;
	float speed_factor=0.0f;
;};


