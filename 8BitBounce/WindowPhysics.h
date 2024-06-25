#pragma once
#include "RigidBody.h"

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

private:
    UINT timerID;
};
