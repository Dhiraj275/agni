#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <optional>
#include <windows.h>
#include <string>
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
class Window {
public:
    Window(int width, int height, std::wstring title);
    ~Window();
	HWND GetHandle() const { return hWnd; }  // Get window handle
	static std::optional<int> ProcessMessage();

public:
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
	Keyboard kbd;
	Mouse mouse;
	Graphics& Gfx() const;
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    HINSTANCE hInstance;
    HWND hWnd;
    std::wstring title;
    int width, height;
	std::unique_ptr<Graphics> pGfx;
	

};

#endif // WINDOW_H
