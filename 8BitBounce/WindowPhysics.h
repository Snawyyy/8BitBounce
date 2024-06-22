#pragma once
#include "RigidBody.h"

class WindowPhysics : public RigidBody
{
public:
	WindowPhysics(HWND WindowHandle) :RigidBody(WindowHandle)
	{

	}

	void UpdateSize() override
	{
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);
		width = windowRect.right - windowRect.left;
		height = windowRect.bottom - windowRect.top;
	}

	void RunPhysics() override
	{
		RigidBody::RunPhysics();
		SetWindowPos(hWnd, NULL, bodyX, bodyY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

};