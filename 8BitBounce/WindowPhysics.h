#pragma once
#include "RigidBody.h"

class WindowPhysics : public RigidBody
{
public:

	bool hasGravity;

	WindowPhysics(HWND WindowHandle) :RigidBody(WindowHandle)
	{
		timerID = 0;
	}

	void SetTimerID(UINT id) { timerID = id; }
	UINT GetTimerID() const { return timerID; }

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

private:
	UINT timerID;
};