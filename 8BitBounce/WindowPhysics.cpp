#include "WindowPhysics.h"

WindowPhysics::WindowPhysics(HWND WindowHandle) : RigidBody(WindowHandle)
{
    timerID = 0;
    UpdateSize();

    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);
    width = windowRect.right - windowRect.left;

    height = windowRect.bottom - windowRect.top;
    body.radius = width / 2;
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
    width = body.radius * 2;
    height = body.radius * 2;
}

void WindowPhysics::RunPhysics()
{
    RigidBody::RunPhysics();
    ApplyCollisions();
    MemoryManager& worldObjects = MemoryManager::getInstance();
    worldObjects.WriteToMemory(GetId(), body);
    SetWindowPos(hWnd, NULL, body.pos.x, body.pos.y, width, height, SWP_NOSIZE | SWP_NOZORDER);
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
            int bodyRadius = width / 2;
            int otherBodyRadius = width / 2;

            physicsObj other = worldObjects.ReadMemory(i);

            if (isColliding(other))
            {
                CalculateCollisions(other);
            }
            double distanceY = std::abs(body.pos.y - other.pos.y);
            double distanceX = std::abs(body.pos.x - other.pos.x);

            if (distanceY != 0 && distanceX != 0)
            {
                ApplyGravity(other);
            }
        }
    }
}

bool WindowPhysics::isColliding(const physicsObj& other)
{
    float dx = body.pos.x - other.pos.x;
    float dy = body.pos.y - other.pos.y;

    float distanceSquared = dx * dx + dy * dy;

    float sumOfRadii = body.radius + other.radius;
    float radiusSquared = sumOfRadii * sumOfRadii;

    return distanceSquared <= radiusSquared;
}

