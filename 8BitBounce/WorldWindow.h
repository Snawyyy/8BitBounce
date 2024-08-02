#pragma once
#include <Windows.h>
#include "UiElements.h"
#include <thread>
#include "MemoryManager.h"
#include "DesktopItemWindow.h"

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

#define WM_UPDATE_DELTA_TIME (WM_USER + 1)


