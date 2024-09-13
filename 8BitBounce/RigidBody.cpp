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

    Vector2 acceleration = { force.x / body.mass, force.y / body.mass };

    body.velocity.x += acceleration.x * deltaTime;
    body.velocity.y += acceleration.y * deltaTime;

    body.pos.x += body.velocity.x * deltaTime;
    body.pos.y += body.velocity.y * deltaTime;

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

void RigidBody::ApplyAirResistance()
{
    // Calculate the speed (magnitude of velocity)
    float speed = sqrt(body.velocity.x * body.velocity.x + body.velocity.y * body.velocity.y);


    if (speed > 0.0f)
    {
        // Calculate the drag force magnitude
        float dragMagnitude = 0.5f * airDensity * speed * speed * dragCoefficient * crossSectionalArea;

        // Calculate the drag force components (opposite to velocity direction)
        float dragForceX = -(body.velocity.x / speed) * dragMagnitude;
        float dragForceY = -(body.velocity.y / speed) * dragMagnitude;

        // Apply the drag force to the total force
        force.x += dragForceX;
        force.y += dragForceY;
    }
}

void RigidBody::ApplyFriction()
{
    // Calculate the normal force (assuming vertical gravity)
    float normalForce = body.mass * gravityAcceleration;

    // Calculate the maximum friction force magnitude
    float maxFrictionForce = frictionCoefficient * normalForce;

    // Get the magnitude of the velocity
    float speed = sqrt(body.velocity.x * body.velocity.x + body.velocity.y * body.velocity.y);

    if (speed > 0.1f)
    {
        // Calculate the friction force components
        float frictionForceX = -(body.velocity.x / speed) * maxFrictionForce;
        float frictionForceY = -(body.velocity.y / speed) * maxFrictionForce;

        // Apply the friction force
        force.x += frictionForceX;
        force.y += frictionForceY;
    }
    else
    {
        // Stop the object completely
        body.velocity.x = 0.0f;
        body.velocity.y = 0.0f;
    }
}

void RigidBody::BorderCollisions()
{
    // Collision with left and right of screen
    if (body.pos.x + body.radius * 2 > screenWidth)
    {
        body.velocity.x = -(body.velocity.x * dampingFactor * body.restitution); // Apply friction and restitution on bounce
        body.pos.x = screenWidth - body.radius * 2; // Prevents the body from getting stuck right to the screen
        ApplyFriction();
    }
    if (body.pos.x < 0)
    {
        body.velocity.x = -(body.velocity.x * dampingFactor * body.restitution); // Apply friction and restitution on bounce
        body.pos.x = 0; // Prevents the body from getting stuck left to the screen
        ApplyFriction();
    }

    // Collision with top and bottom of screen
    if (body.pos.y + body.radius * 2 > screenHeight - taskbarHeight)
    {
        body.velocity.y = -(body.velocity.y * dampingFactor * body.restitution);
        body.pos.y = screenHeight - taskbarHeight - body.radius * 2; // Prevents the body from getting stuck below the screen
        ApplyFriction();
    }
    if (body.pos.y < 0)
    {
        body.velocity.y = -(body.velocity.y * dampingFactor * body.restitution);
        body.pos.y = 0; // Prevents the body from getting stuck above the screen
        ApplyFriction();
    }
}

void RigidBody::Draggable()
{
    if (isDragging)
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        float cursorX = static_cast<float>(cursorPos.x);
        float cursorY = static_cast<float>(cursorPos.y);

        mouseJointTarget.x = cursorX - clickOffset.x;
        mouseJointTarget.y = cursorY - clickOffset.y;
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
    // Calculate relative position
    Vector2 relativePosition = {
        other.pos.x - body.pos.x,
        other.pos.y - body.pos.y
    };

    float distanceSquared = relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y;
    float distance = sqrt(distanceSquared);

    // Check if collision occurs
    float radiusSum = body.radius + other.radius;
    if (distance >= radiusSum || distance == 0.0f) return;

    // Collision normal
    Vector2 normal = {
        relativePosition.x / distance,
        relativePosition.y / distance
    };

    // Resolve interpenetration (Positional Correction)
    float penetrationDepth = radiusSum - distance;
    float totalMass = body.mass + other.mass;

    // Positional correction factor
    const float percent = 0.8f; // Penetration percentage to correct (usually 20% - 80%)
    const float slop = 0.01f;   // Penetration allowance
    float correctionMagnitude = (penetrationDepth - slop) / totalMass * percent;

    if (penetrationDepth > slop)
    {
        body.pos.x -= correctionMagnitude * other.mass * normal.x;
        body.pos.y -= correctionMagnitude * other.mass * normal.y;
        other.pos.x += correctionMagnitude * body.mass * normal.x;
        other.pos.y += correctionMagnitude * body.mass * normal.y;
    }

    // Relative velocity
    Vector2 relativeVelocity = {
        other.velocity.x - body.velocity.x,
        other.velocity.y - body.velocity.y
    };

    // Velocity along the normal
    float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0) return;

    // Calculate restitution (use the minimum restitution)
    float e = fmin(body.restitution, other.restitution);

    // Calculate impulse scalar
    float inverseMass1 = 1.0f / body.mass;
    float inverseMass2 = 1.0f / other.mass;
    float impulseScalar = -(1 + e) * velocityAlongNormal / (inverseMass1 + inverseMass2);

    // Apply impulse to the bodies
    Vector2 impulse = {
        impulseScalar * normal.x,
        impulseScalar * normal.y
    };

    body.velocity.x -= inverseMass1 * impulse.x;
    body.velocity.y -= inverseMass1 * impulse.y;
    other.velocity.x += inverseMass2 * impulse.x;
    other.velocity.y += inverseMass2 * impulse.y;
}

void RigidBody::CalculateCollisionsWithWindow(const WindowInfo& window)
{
    RECT windowRect = window.rect;

    float closestX = Clamp(body.pos.x, (float)windowRect.left - body.radius, (float)windowRect.right - body.radius);
    float closestY = Clamp(body.pos.y, (float)windowRect.top - body.radius, (float)windowRect.bottom - body.radius);

    Vector2 relativePosition = {
        closestX - body.pos.x,
        closestY - body.pos.y
    };

    float distSq = relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y;

    if (distSq == 0.0f) return;

    Vector2 normal = {
        relativePosition.x / sqrt(distSq),
        relativePosition.y / sqrt(distSq)
    };

    float penetrationDepth = body.radius - sqrt(distSq);
    if (penetrationDepth > 0) {
        // For window collision, we only move the body (the window doesn't move)
        float correctionFactor = penetrationDepth;
        body.pos.x -= correctionFactor * normal.x;
        body.pos.y -= correctionFactor * normal.y;
    }

    // Treat the window as an object with infinite mass
    Vector2 relativeVelocity = {
        -body.velocity.x,
        -body.velocity.y
    };

    float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

    if (velocityAlongNormal > 0) return;

    float impactFactor = (1 + body.restitution) * velocityAlongNormal;

    body.velocity.x += impactFactor * normal.x;
    body.velocity.y += impactFactor * normal.y;

    ApplyFriction();
}


void RigidBody::RunPhysics()
{
    CalculateTime();

    // Update draggable target position
    Draggable();

    // Apply forces
    if (worldGravity)
    {
        ApplyWorldGravity();
    }
    ApplyAirResistance();
    ApplyFriction();
    ApplyMouseJoint(); // Apply the mouse joint force

    // Update velocities based on forces
    Vector2 acceleration = { force.x / body.mass, force.y / body.mass };
    body.velocity.x += acceleration.x * deltaTime;
    body.velocity.y += acceleration.y * deltaTime;

    // Clear forces after applying
    force = Vector2{ 0.0f, 0.0f };

    // Update positions
    body.pos.x += body.velocity.x * deltaTime;
    body.pos.y += body.velocity.y * deltaTime;

    // Handle collisions
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

    // Store the initial click position
    initialClickPosition = cursorPos;

    // Set the target position for the mouse joint
    mouseJointTarget.x = body.pos.x;
    mouseJointTarget.y = body.pos.y;

    SetCapture(hWnd);
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
    isDragging = FALSE;
    ReleaseCapture();
}

void RigidBody::ApplyMouseJoint()
{
    if (isDragging)
    {
        // Calculate the vector from the body to the target
        Vector2 toTarget = {
            mouseJointTarget.x - body.pos.x,
            mouseJointTarget.y - body.pos.y
        };

        // Calculate the distance to the target
        float distance = sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

        if (distance > 0.0f)
        {
            // Normalize the direction
            Vector2 direction = {
                toTarget.x / distance,
                toTarget.y / distance
            };

            // Define the spring constant (stiffness) and damping factor
            const float stiffness = 2000.0f; // Adjust as needed
            const float damping = 50.0f;    // Adjust as needed

            // Calculate the relative velocity between the body and the target (assumed to be stationary)
            Vector2 relativeVelocity = {
                body.velocity.x,
                body.velocity.y
            };

            // Calculate the spring force
            float springForceMagnitude = stiffness * distance;

            Vector2 springForce = {
                springForceMagnitude * direction.x,
                springForceMagnitude * direction.y
            };

            // Calculate the damping force
            Vector2 dampingForce = {
                damping * relativeVelocity.x,
                damping * relativeVelocity.y
            };

            float scalingCoefficient = 0.00000000001f;
            // Scale factor that weakens the force based on mass
            float scaleFactor = 1.0f / (1.0f + body.mass * scalingCoefficient);

            // Adjusted force calculation
            force.x += ((springForce.x - dampingForce.x) * body.mass * scaleFactor);
            force.y += ((springForce.y - dampingForce.y) * body.mass * scaleFactor);
        }
    }
}


float RigidBody::Clamp(float num, float min, float max)
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
