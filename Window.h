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
#include "AgniException.h"


//some helper macros
#define AGWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define AGWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )


class Window {
public:
    Window(int width, int height, std::wstring title);
    ~Window();
	HWND GetHandle() const { return hWnd; }  // Get window handle
	static std::optional<int> ProcessMessage();

private:
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
	class Exception : public AgniException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

public:
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
