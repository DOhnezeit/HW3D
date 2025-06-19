#include <Windows.h>

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
	Wc.lpfnWndProc = DefWindowProc;
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

	while (true);

	return 0;
}