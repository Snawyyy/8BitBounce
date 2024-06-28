#pragma once
#include "RigidBody.h"
#include "WindowIdManager.h"
#include "MemoryManager.h"

class WindowPhysics : public RigidBody
{
public:
    bool hasGravity;
    int id;

    WindowPhysics(HWND WindowHandle);

    void SetTimerID(UINT id);
    UINT GetTimerID() const;

    void UpdateSize() override;
    void RunPhysics() override;
    int GetId();


private:
    UINT timerID;
};
