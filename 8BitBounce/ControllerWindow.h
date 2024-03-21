#pragma once
#include <Windows.h>
#include <thread>
#include "CustomWindowUI.h"
#include "BallWindow.h"

using namespace std;
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
