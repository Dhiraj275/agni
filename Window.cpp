#include <iostream>
#include <optional>
#include  <string>
#include "Window.h"
#include "Resource.h"


Window::WindowClass Window::WindowClass::wndClass;
Window::WindowClass::WindowClass() noexcept
    :
    hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;
    wc.hIcon = static_cast<HICON>(LoadImage(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const WCHAR* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}


Window::Window(int width, int height, std::wstring title)
    :width(width), height(height), title(title) {

    hwnd = CreateWindowEx(0, Window::WindowClass::GetName(), title.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, WindowClass::GetInstance(), this);

    if (!hwnd) {
        MessageBox(nullptr, L"Window Creation Failed!", L"Error", MB_OK);
        exit(-1);
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
}

Window::~Window() {
    DestroyWindow(hwnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window class handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window class
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    //testing
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

        //keyboard
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
        {
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_SYSKEYUP:
    case WM_KEYUP:
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        break;
    case WM_KILLFOCUS:
        kbd.ClearState();
        break;

        //mouse messages
    case WM_MOUSEMOVE: {
        const POINTS p = MAKEPOINTS(lParam);
        mouse.OnMouseMove(p.x, p.y);
        break;
    }
    case WM_LBUTTONDOWN: {
        const POINTS p = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(p.x, p.y);
        break;
    }
    case WM_RBUTTONDOWN: {
        const POINTS p = MAKEPOINTS(lParam);
        mouse.OnRightPressed(p.x, p.y);
        break;
    }
    case WM_LBUTTONUP: {
        const POINTS p = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(p.x, p.y);
        break;
    }
    case WM_RBUTTONUP: {
        const POINTS p = MAKEPOINTS(lParam);
        mouse.OnRightReleased(p.x, p.y);
        break;
    }
    case WM_MOUSEWHEEL: {


        const POINTS p = MAKEPOINTS(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
            mouse.OnWheelUp(p.x, p.y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
            mouse.OnWheelDown(p.x, p.y);
        }
        break;
    }
    }

   
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessage() {
    MSG msg;
    while ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) > 0)
    {
        if (msg.message == WM_QUIT) {
            return msg.wParam;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    return {};
}