#include "Window.h"

#include <sstream>

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
	RECT wr{};
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	// Calculates the actual window size where the "client" window size is the one you pass into the function, for example a 800x600 window needs extra space for borders/header etc.
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	Width = wr.right - wr.left;	 //	adjusted width
	Height = wr.bottom - wr.top; // and height

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
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data to store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Set message procedure to normal (non-setup) handler after setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMessageRedirect));

		// Send message to the window class handler
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMessageRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Get pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Send message to window class handler
	return pWnd->HandleMessage(hWnd, msg, wParam, lParam);

	return LRESULT();
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WindowException::WindowException(int line, const char* file, HRESULT hr) noexcept
	: CustomException(line, file), hr(hr)
{}

const char* Window::WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();

	return whatBuffer.c_str();
}

const char* Window::WindowException::GetType() const noexcept
{
	return "Window Exception";
}

std::string Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr
	);

	if (nMsgLen == 0)
	{
		return "Unknown error code";
	}

	std::string ErrorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return ErrorString;
}

HRESULT Window::WindowException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::WindowException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
