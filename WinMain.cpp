#include "Win.h" 

#include "WindowsMessageLogger.h"
#include "Window.h"

#include <sstream>
#include <string>

void OnKeyPress(HWND hWnd, WPARAM wParam)
{
	static std::wstring title;

	if (wParam == VK_BACK)
	{
		if (!title.empty())
			title.pop_back();
	}
	else if (wParam >= L' ' && wParam != 0x7F) // Filter out control chars and DEL
	{
		title.push_back(static_cast<wchar_t>(wParam));
	}

	SetWindowText(hWnd, title.c_str());
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageLogger ML;
	OutputDebugString(ML(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, L"You pressed F");
		}
		break;
	case WM_CHAR:
		{
			OnKeyPress(hWnd, wParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			std::wostringstream oss;
			oss << "(" << pt.x << "," << pt.y << ")";
			std::wstring text = oss.str();
			SetWindowText(hWnd, text.c_str());
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(_In_		HINSTANCE hInstance,
				   _In_opt_	HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine,
				   _In_ int nCmdShow)
{
	Window Wnd(800, 600, L"This is a window!");
	
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;
}

