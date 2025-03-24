#ifndef UNICODE
#define UNICODE
#endif 
#include "App.h"
#include <sstream>
int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow) {
	
	App{}.Go();
 }