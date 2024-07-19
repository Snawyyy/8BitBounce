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
    force.y += G * (body.mass * EARTH_MASS) / (EARTH_RADIUS * EARTH_RADIUS) * SECOND_TO_MILISECOND;
}

void RigidBody::ApplyGravity(physicsObj other)
{
    float directionX = other.pos.x - body.pos.x;
    float directionY = other.pos.y - body.pos.y;
    float distanceSquared = directionX * directionX + directionY * directionY;
    float distance = sqrt(distanceSquared);

    if (distance > 0) 
    {
        float normalizedX = directionX / distance;
        float normalizedY = directionY / distance;

        // Calculate gravitational force
        float forceMagnitude = G * (body.mass * other.mass) / distanceSquared;

        // Apply force components
        force.x += forceMagnitude * normalizedX * SECOND_TO_MILISECOND;
        force.y += forceMagnitude * normalizedY * SECOND_TO_MILISECOND;
    }
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
    if (body.pos.x + body.radius * 2 > screenWidth)
    {
        body.velocity.x = -(body.velocity.x * dampingFactor * body.restitution); // Apply friction and restitution on bounce
        body.pos.x = screenWidth - body.radius * 2; // Prevents the body from getting stuck right to the screen
    }
    if (body.pos.x < 0)
    {
        body.velocity.x = -(body.velocity.x * dampingFactor * body.restitution); // Apply friction and restitution on bounce
        body.pos.x = 0; // Prevents the body from getting stuck left to the screen
    }

    // Collision with top and bottom of screen
    if (body.pos.y + body.radius * 2 > screenHeight - taskbarHeight)
    {
        body.velocity.y = -(body.velocity.y * dampingFactor * body.restitution);
        body.pos.y = screenHeight - taskbarHeight - body.radius * 2; // Prevents the body from getting stuck below the screen
    }
    if (body.pos.y < 0)
    {
        body.velocity.y = -(body.velocity.y * dampingFactor * body.restitution);
        body.pos.y = 0; // Prevents the body from getting stuck above the screen
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

void RigidBody::CalculateCollisions(physicsObj& other)
{

    Vector2 relativeVelocity = {
        other.velocity.x - body.velocity.x,
        other.velocity.y - body.velocity.y
    };

    Vector2 relativePosition = {
        other.pos.x - body.pos.x,
        other.pos.y - body.pos.y
    };

    float distSq = relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y;

    if (distSq == 0.0f) return;

    Vector2 normal = {
        relativePosition.x / sqrt(distSq),
        relativePosition.y / sqrt(distSq)
    };

    float penetrationDepth = (body.radius + other.radius) - sqrt(distSq);
    if (penetrationDepth > 0) {
        float correctionFactor = 0.5f * penetrationDepth / (body.mass + other.mass);
        body.pos.x -= correctionFactor * other.mass * normal.x;
        body.pos.y -= correctionFactor * other.mass * normal.y;
        other.pos.x += correctionFactor * body.mass * normal.x;
        other.pos.y += correctionFactor * body.mass * normal.y;
    }

    float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

    if (velocityAlongNormal > 0) return;

    float impactFactor = (other.restitution + body.restitution) * velocityAlongNormal / (body.mass + other.mass);

    body.velocity.x += impactFactor * other.mass * normal.x;
    body.velocity.y += impactFactor * other.mass * normal.y;
    other.velocity.x -= impactFactor * body.mass * normal.x;
    other.velocity.y -= impactFactor * body.mass * normal.y;
}


void RigidBody::RunPhysics()
{
    CalculateTime();
    UpdatePosition();
    if (worldGravity) 
    {
        ApplyWorldGravity(); 
        ApplyFriction();
    }
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
