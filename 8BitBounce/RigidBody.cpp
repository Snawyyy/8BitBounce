#include "RigidBody.h"

RigidBody::RigidBody(HWND windowHandle) : Physics(windowHandle)
{
    bodyX = centerW;
    preBodyX = centerW;
    preBodyY = centerH;
}

void RigidBody::UpdatePosition()
{
    preBodyX = bodyX;
    preBodyY = bodyY;

    // Calculate acceleration from the applied force
    Vector2 acceleration = { force.x / mass, force.y / mass };

    // Update velocity based on acceleration
    bodyVx += acceleration.x * deltaTime;
    bodyVy += acceleration.y * deltaTime;

    // Update position based on velocity
    bodyX += bodyVx * deltaTime;
    bodyY += bodyVy * deltaTime;

    // Reset the applied force after updating the position
    force = Vector2{ 0.0f, 0.0f };
}

void RigidBody::ApplyWorldGravity()
{
    force.y += gravity * mass * SECOND_TO_MILISECOND;
}

void RigidBody::ApplyGravity(double m1, double m2, double r)
{
    force.y += G * (m1 * m2) / (r * r) * SECOND_TO_MILISECOND;
}

void RigidBody::ApplyFriction()
{
    Vector2 frictionForce = Vector2{ -bodyVx * friction, -bodyVy * friction };
    force.x += frictionForce.x;
    force.y += frictionForce.y;
}

void RigidBody::BorderCollisions()
{
    // Collision with left and right of screen
    if (bodyX + width > screenWidth)
    {
        bodyVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
        bodyX = screenWidth - width; // Prevents the body from getting stuck right to the screen
        force.x += 2.0f * bodyVx * mass; // Apply impulse force
    }
    if (bodyX < 0)
    {
        bodyVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
        bodyX = 0; // Prevents the body from getting stuck left to the screen
        force.x -= 2.0f * bodyVx * mass; // Apply impulse force
    }

    // Collision with top and bottom of screen
    if (bodyY + height > screenHeight - taskbarHeight)
    {
        bodyVy = -(bodyVy * dampingFactor * restitution);
        bodyY = screenHeight - taskbarHeight - height; // Prevents the body from getting stuck below the screen
        force.y += 2.0f * bodyVy * mass; // Apply impulse force
    }
    if (bodyY < 0)
    {
        bodyVy = -(bodyVy * dampingFactor * restitution);
        bodyY = 0; // Prevents the body from getting stuck above the screen
        force.y -= 2.0f * bodyVy * mass; // Apply impulse force
    }
}

void RigidBody::Draggable()
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    if (isDragging)
    {
        bodyX = cursorPos.x - clickOffset.x;
        bodyY = cursorPos.y - clickOffset.y;
    }
}

int RigidBody::GetTaskbarHeight()
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

void RigidBody::RunPhysics()
{
    UpdatePosition();
    ApplyGravity(mass, EARTH_MASS, EARTH_RADIUS);
    ApplyFriction();
    Draggable();
    BorderCollisions();
}

void RigidBody::ApplyForce(float addForceX, float addForceY)
{
    force.x += addForceX;
    force.y += addForceY;
}

void RigidBody::Grab()
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    clickOffset.x = cursorPos.x - bodyX;
    clickOffset.y = cursorPos.y - bodyY;
    isDragging = TRUE;

    // Apply an impulse force to cancel out the existing velocity
    force.x -= bodyVx * mass;
    force.y -= bodyVy * mass;

    SetCapture(hWnd);
    UpdateSize();
}

void RigidBody::TrackGrabbing()
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE; // Specifies that we want a WM_MOUSELEAVE message when the mouse leaves
    tme.hwndTrack = hWnd;
    tme.dwHoverTime = HOVER_DEFAULT; // Not needed for WM_MOUSELEAVE but required to be set

    TrackMouseEvent(&tme); // Call this function to start tracking
}

void RigidBody::Ungrab()
{
    // Update the button's pressed state
    isDragging = FALSE;

    // Calculate the velocity from the change in position
    float velocityScaleFactor = 50.0f; // Adjust this value as needed
    bodyVx = (bodyX - preBodyX) * velocityScaleFactor;
    bodyVy = (bodyY - preBodyY) * velocityScaleFactor;

    // Apply an impulse force based on the calculated velocity
    force.x += bodyVx * mass;
    force.y += bodyVy * mass;

    // Release the capture
    ReleaseCapture();
}
