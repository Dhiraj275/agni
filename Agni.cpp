#ifndef UNICODE
#define UNICODE
#endif 
#include "Window.h"
#include <sstream>
int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow) {
	
	Window window(1280, 720, L"Fire Engine");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		while (!window.mouse.IsEmpty()) {
			Mouse::Event e = window.mouse.Read();
			if (e.GetType() == Mouse::Event::Type::Move) {
				std::ostringstream  oss;
				oss<< "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				window.SetTitle(oss.str());
			}
			else if (e.GetType() == Mouse::Event::Type::LPress) {
				std::ostringstream  oss;
				oss << "Left Click at: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				window.SetTitle(oss.str());
			}
			else if (e.GetType() == Mouse::Event::Type::WheelUp) {
				std::ostringstream  oss;
				oss << "Wheel Up at: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				window.SetTitle(oss.str());
			}
			else if (e.GetType() == Mouse::Event::Type::WheelDown) {
				std::ostringstream  oss;
				oss << "Wheel Down at: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				window.SetTitle(oss.str());
			}
		}

	}
	
	if (gResult == -1)
	{
		return -1;
	}
 }