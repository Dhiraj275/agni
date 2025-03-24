#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <string>
#include "Keyboard.h"
#include "Mouse.h"

class Window {
public:
    Window(int width, int height, std::wstring title);
    ~Window();
    HWND GetHandle() const { return hwnd; }  // Get window handle
	class WindowClass
	{
	public:
		static const WCHAR* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const WCHAR* wndClassName = L"Agni";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Keyboard kbd;
	Mouse mouse;
	//testing
	void SetTitle(const std::string& title) noexcept;
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    HINSTANCE hInstance;
    HWND hwnd;
    std::wstring title;
    int width, height;

	

};

#endif // WINDOW_H
