#pragma once
#include <Windows.h>
#include "RigidBody.h"
#include "WorldWindow.h"

class Window
{
public:
	Window();
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
