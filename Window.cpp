#include "Window.h"

Window::WindowClass Window::WindowClass::WndClass;

Window::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	// Register window class
	WNDCLASSEX Wc = { 0 };
	Wc.cbSize = sizeof(Wc);
	Wc.style = CS_OWNDC;
	Wc.lpfnWndProc = HandleMessageSetup;
	Wc.cbClsExtra = 0;
	Wc.cbWndExtra = 0;
	Wc.hInstance = GetInstance();
	Wc.hIcon = nullptr;
	Wc.hCursor = nullptr;
	Wc.hbrBackground = nullptr;
	Wc.lpszMenuName = nullptr;
	Wc.lpszClassName = GetName();
	Wc.hIconSm = nullptr;

	RegisterClassEx(&Wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(WindowClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName()
{
	return WindowClassName;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return WndClass.hInst;
}

// Window things

Window::Window(int width, int height, const wchar_t* name) noexcept
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	// Calculates the actual window size where the "client" window size is the one you pass into the function, for example a 800x600 window needs extra space for borders/header etc.
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// Create the window and get the hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{ 

}

LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

LRESULT WINAPI Window::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
