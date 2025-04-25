#include <iostream>
#include <optional>
#include  <string>
#include  <sstream>
#include "ImGui/imgui_impl_win32.h"
#include "Window.h"
#include "Resource.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


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
        hWnd = CreateWindowEx(0, Window::WindowClass::GetName(), title.c_str(),
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            nullptr, nullptr, WindowClass::GetInstance(), this);

        ShowWindow(hWnd, SW_SHOWDEFAULT);
        pGfx = std::make_unique<Graphics>(hWnd);   
        ImGui_ImplWin32_Init(hWnd);
}

Window::~Window() {
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd);
}

Graphics& Window::Gfx() const
{
    return *pGfx;
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
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

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

//exception handling
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
:
    AgniException(line, file),
    hr(hr)
{
    
}

const char* Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
    return "Agni Window Exception";
}


std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );
    // 0 string length returned indicates a failure
    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(hr);
}
