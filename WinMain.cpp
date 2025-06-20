#include <Windows.h>
#include "WindowsMessageLogger.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageLogger ML;
	OutputDebugString(ML(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(_In_		HINSTANCE hInstance,
				   _In_opt_	HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine,
				   _In_ int nCmdShow)
{
	const LPCWSTR pClassName = L"HW3D";

	// Register window class
	WNDCLASSEX Wc = { 0 };
	Wc.cbSize = sizeof(Wc);
	Wc.style = CS_OWNDC;
	Wc.lpfnWndProc = WndProc;
	Wc.cbClsExtra = 0;
	Wc.cbWndExtra = 0;
	Wc.hInstance = hInstance;
	Wc.hIcon = nullptr;
	Wc.hCursor = nullptr;		
	Wc.hbrBackground = nullptr;
	Wc.lpszMenuName = nullptr;
	Wc.lpszClassName = pClassName;
	Wc.hIconSm = nullptr;
	
	
	
	RegisterClassEx(&Wc);
	
	// Create window instance
	HWND hWnd = CreateWindowExW(
		0, pClassName, L"This is a window", 
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 800, 600,	
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	MSG Msg;
	BOOL gResult;

	while (gResult = (GetMessage(&Msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
	}

	if (gResult == -1)
	{
		return - 1;
	}

	return Msg.wParam;
}

