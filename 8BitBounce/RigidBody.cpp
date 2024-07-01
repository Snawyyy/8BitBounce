#include "RigidBody.h"

RigidBody::RigidBody(HWND windowHandle) : Physics(windowHandle)
{
    body.pos.x = centerW;
    body.pos.y = centerH;

    preBodyX = centerW;
    preBodyY = centerH;
    body.mass = 1.0f;

}

void RigidBody::CalculateTime()
{
    currentTime = GetTickCount64();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
}

void RigidBody::UpdatePosition()
{
    preBodyX = body.pos.x;
    preBodyY = body.pos.y;

    // Calculate acceleration from the applied force
    Vector2 acceleration = { force.x / body.mass, force.y / body.mass };

    // Update velocity based on acceleration
    body.velocity.x += acceleration.x * deltaTime;
    body.velocity.y += acceleration.y * deltaTime;

    // Update position based on velocity
    body.pos.x += body.velocity.x * deltaTime;
    body.pos.y += body.velocity.y * deltaTime;

    // Reset the applied force after updating the position
    force = Vector2{ 0.0f, 0.0f };
}

void RigidBody::ApplyWorldGravity()
{
    force.y += gravity * body.mass * SECOND_TO_MILISECOND;
}

void RigidBody::ApplyGravity(double m1, double m2, double r)
{
    force.y += G * (m1 * m2) / (r * r) * SECOND_TO_MILISECOND;
}

void RigidBody::ApplyFriction()
{
    Vector2 frictionForce = Vector2{ -body.velocity.x * friction, -body.velocity.y * friction };
    force.x += frictionForce.x;
    force.y += frictionForce.y;
}

void RigidBody::BorderCollisions()
{
    // Collision with left and right of screen
    if (body.pos.x + width > screenWidth)
    {
        body.velocity.x = -(body.velocity.x * dampingFactor * restitution); // Apply friction and restitution on bounce
        body.pos.x = screenWidth - width; // Prevents the body from getting stuck right to the screen
        force.x += 2.0f * body.velocity.x * body.mass; // Apply impulse force
    }
    if (body.pos.x < 0)
    {
        body.velocity.x = -(body.velocity.x * dampingFactor * restitution); // Apply friction and restitution on bounce
        body.pos.x = 0; // Prevents the body from getting stuck left to the screen
        force.x -= 2.0f * body.velocity.x * body.mass; // Apply impulse force
    }

    // Collision with top and bottom of screen
    if (body.pos.y + height > screenHeight - taskbarHeight)
    {
        body.velocity.y = -(body.velocity.y * dampingFactor * restitution);
        body.pos.y = screenHeight - taskbarHeight - height; // Prevents the body from getting stuck below the screen
        force.y += 2.0f * body.velocity.y * body.mass; // Apply impulse force
    }
    if (body.pos.y < 0)
    {
        body.velocity.y = -(body.velocity.y * dampingFactor * restitution);
        body.pos.y = 0; // Prevents the body from getting stuck above the screen
        force.y -= 2.0f * body.velocity.y * body.mass; // Apply impulse force
    }
}

void RigidBody::Draggable()
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    if (isDragging)
    {
        body.pos.x = cursorPos.x - clickOffset.x;
        body.pos.y = cursorPos.y - clickOffset.y;
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

void RigidBody::CalculateCollisions(physicsObj other)
{
    body.velocity.x = (-body.velocity.x - other.velocity.x) * restitution;
    body.velocity.y = (-body.velocity.y - other.velocity.y) * restitution;
}

void RigidBody::RunPhysics()
{
    CalculateTime();
    UpdatePosition();
    ApplyGravity(body.mass, EARTH_MASS, EARTH_RADIUS);
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
    clickOffset.x = cursorPos.x - body.pos.x;
    clickOffset.y = cursorPos.y - body.pos.y;
    isDragging = TRUE;

    // Apply an impulse force to cancel out the existing velocity
    force.x -= body.velocity.x * body.mass;
    force.y -= body.velocity.y * body.mass;

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
    body.velocity.x = (body.pos.x - preBodyX) * velocityScaleFactor;
    body.velocity.y = (body.pos.y - preBodyY) * velocityScaleFactor;

    // Apply an impulse force based on the calculated velocity
    force.x += body.velocity.x * body.mass;
    force.y += body.velocity.y * body.mass;

    // Release the capture
    ReleaseCapture();
}

int RigidBody::Clamp(int num, int min, int max)
{
    if (num < min) {
        return min;
    }
    else if (num > max) {
        return max;
    }
    else {
        return num;
    }
}
