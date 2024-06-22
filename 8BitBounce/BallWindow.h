#pragma once
#include <Windows.h>
#include "WindowPhysics.h"
#include "WorldWindow.h"
#include "DropDownOptions.h"

class Window
{
public:
	Window(int width, int height);
	
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void Show();

	~Window();

	void ProcessMessages();

private:
	HINSTANCE m_hinstance;
	HWND m_hWnd;
};

int GetTaskbarHeight();

void changeColorRandomly();
