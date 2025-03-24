#pragma once
#include "Window.h"
#include "AgniTimer.h"
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
;};


