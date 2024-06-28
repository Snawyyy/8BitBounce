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
    ApplyCollisions();
    MemoryManager& worldObjects = MemoryManager::getInstance();
    worldObjects.WriteToMemory(GetId(), bodyX);
    SetWindowPos(hWnd, NULL, bodyX, bodyY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

int WindowPhysics::GetId()
{
    WindowIdManager& idManager = WindowIdManager::getInstance();
    return idManager.getWindowId(hWnd);
}

void WindowPhysics::ApplyCollisions()
{
    WindowIdManager& idManager = WindowIdManager::getInstance();
    MemoryManager& worldObjects = MemoryManager::getInstance();

    for (size_t i = 1; i <= idManager.objectsCount; i++)
    {
        if (i != GetId())
        {
            int diraction = Clamp(bodyX - worldObjects.ReadMemory(i), -1, 1);

            int bodyRadius = width / 2;
            int otherBodyRadius = width / 2;

            if (isColliding(bodyX, bodyRadius, worldObjects.ReadMemory(i), otherBodyRadius))
            {
                CalculateCollisions();
            }
        }
    }
}

bool WindowPhysics::isColliding(float bodyX, float bodyRadius, float otherBodyX, float otherBodyRadius) {
    float leftEdge = bodyX - bodyRadius;
    float rightEdge = bodyX + bodyRadius;
    float otherLeftEdge = otherBodyX - otherBodyRadius;
    float otherRightEdge = otherBodyX + otherBodyRadius;

    return !(leftEdge > otherRightEdge || rightEdge < otherLeftEdge);
}

