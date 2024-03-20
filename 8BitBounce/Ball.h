#pragma once
#include <Windows.h>


class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();

	bool ProcessMessages();

private:
	HINSTANCE m_hinstance;
	HWND m_hWnd;
};