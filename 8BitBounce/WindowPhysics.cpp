#include "WindowPhysics.h"

WindowPhysics::WindowPhysics(HWND WindowHandle) : RigidBody(WindowHandle)
{
    timerID = 0;
    hasGravity = true; // Default to having gravity
}

void WindowPhysics::SetTimerID(UINT id)
{
    timerID = id;
}

UINT WindowPhysics::GetTimerID() const
{
    return timerID;
}

void WindowPhysics::UpdateSize()
{
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    width = windowRect.right - windowRect.left;
    height = windowRect.bottom - windowRect.top;
}

void WindowPhysics::RunPhysics()
{
    RigidBody::RunPhysics();
    SetWindowPos(hWnd, NULL, bodyX, bodyY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

int WindowPhysics::GetId()
{
    WindowIdManager& idManager = WindowIdManager::getInstance();
    return idManager.getWindowId(hWnd);
}