#pragma once
#include "Window.h"
#include "AgniTimer.h"
#include "Sphere.h"
#include "Test3dModel.h"
#include "Camera.h"
#include "Box.h"
#include "Plane.h"
#include "Cube.h"
#include "DearImGui.h"
#include "PointLight.h"
#include "GeoSphere.h"
#include "Skybox.h"
#include "Terrain.h"
class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window window;
	DearImGui dearImGui;
	Camera cam;
	AgniTimer timer;
	PointLight light;
	//std::unique_ptr<class Skybox> skybox;
	std::vector<std::unique_ptr<class Box >> boxes;
	std::unique_ptr<class Sphere> sphere;
	std::unique_ptr<class GeoSphere> geoSphere;
	std::unique_ptr<class Model3d> model;
	std::unique_ptr<class Plane> plane;
	std::unique_ptr<class Terrain> terrain;

	std::unique_ptr<class Cube> cube;

	float speed_factor=0.0f;
;};


