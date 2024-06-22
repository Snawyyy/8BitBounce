#pragma once
#include <Windows.h>

#define SECOND_TO_MILISECOND 1000
#define VELOCITY_THRESHOLD 10


class Physics
{
private:

		int GetTaskbarHeight()
		{
			APPBARDATA abd;
			memset(&abd, 0, sizeof(abd));
			abd.cbSize = sizeof(abd);

			if (SHAppBarMessage(ABM_GETTASKBARPOS, &abd))
			{
				return abd.rc.bottom - abd.rc.top;
			}
			else
			{
				// If the function fails, return a default value (e.g., 0)
				return 0;
			}
		}

protected:

		virtual void UpdateSize()
		{
		}

	public:

		HWND hWnd;
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int taskbarHeight = GetTaskbarHeight();
		int centerW = screenWidth / 2;
		int centerH = screenHeight / 2;

		int width;
		int height;
		float deltaTime = 0;

		Physics(HWND windowHandle)
		{
			hWnd = windowHandle;
			/*
			if (!IsWindow(hWnd))
			{
				MessageBox(hWnd, L"a", L"", MB_OK);
			}*/
			UpdateSize();
		}

		HWND GetWindowHandle()
		{
			return this->hWnd;
		}

		void SetWindowHandle(HWND handle)
		{
			this->hWnd = handle;
			UpdateSize();
		}

		void virtual RunPhysics()
		{
		}

		void GetDeltaTime(float dt)
		{
			this->deltaTime = dt;
		}
};