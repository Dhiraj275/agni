#pragma once
#include "Window.h"
#include "AgniTimer.h"
#include "Box.h"
class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window window;
	AgniTimer timer;
	std::unique_ptr<class Box > box;
;};


