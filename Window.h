#pragma once
#include "Win.h"



class Window
{
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName();
		static HINSTANCE GetInstance();
	private:
		WindowClass() noexcept;
		~WindowClass();
		
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const wchar_t* WindowClassName = L"Direct3D Engine Window";
		static WindowClass WndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const wchar_t* name) noexcept;
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int Width;
	int Height;
	HWND hWnd;
};

