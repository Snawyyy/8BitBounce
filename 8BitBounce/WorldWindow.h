#pragma once
#include <Windows.h>
#include "BallWindow.h"
#include "UiElements.h"
#include <thread>

class WorldWindow
{
public:
	WorldWindow();
	WorldWindow(const WorldWindow&) = delete;
	WorldWindow& operator=(const WorldWindow&) = delete;
	~WorldWindow();

	bool ProcessMessages();

private:
	HINSTANCE m_hinstance;
	HWND m_hWnd;
};

void CreateWindowThread();

void SendUpdateTimeMessageToAllWindows(float deltaTime);

#define WM_UPDATE_DELTA_TIME (WM_USER + 1)

