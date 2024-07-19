#include "Movements.h"

Movements::Movements(WindowPhysics* b)
{
	movingBody = b;
    lastJump = movingBody->currentTime;
}

Movements::~Movements()
{
}

void Movements::MoveTo(POINT target)
{
    // Calculate the direction vector
    float dx = target.x - (movingBody->body.pos.x + movingBody->body.radius);
    float dy = target.y - (movingBody->body.pos.y + movingBody->body.radius);

    // Calculate the distance to the target
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > movingBody->body.radius * 2) // If not too close to the target
    {
        // Normalize the direction
        dx /= distance;
        dy /= distance;

        // Calculate desired velocity
        float desiredVx = dx * speed;
        float desiredVy = dy * speed;

        // Calculate steering force
        float steeringX = desiredVx - movingBody->body.velocity.x;
        float steeringY = desiredVy - movingBody->body.velocity.y;

        // Apply steering force
        movingBody->ApplyForce(steeringX, steeringY);
    }
    else // Close to target, start slowing down
    {
        // Apply braking force
        float brakingForce = speed * 2;
        float currentSpeed = std::sqrt(movingBody->body.velocity.x * movingBody->body.velocity.x +
            movingBody->body.velocity.y * movingBody->body.velocity.y);

        if (currentSpeed > 0.1f)
        {
            float fx = -movingBody->body.velocity.x / currentSpeed * brakingForce;
            float fy = -movingBody->body.velocity.y / currentSpeed * brakingForce;
            movingBody->ApplyForce(fx, fy);
        }
    }
}

void Movements::GenerateWanderTarget()
{
    // Get the screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a random number generator
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distX(0, screenWidth);
    std::uniform_int_distribution<int> distY(0, screenHeight);

    // Generate a random point on the screen
    WanderTarget.x = distX(rng);
    WanderTarget.y = distY(rng);
}

void Movements::Jump(float jumpForce)
{
    if (IsOnGround() && CanJump())
    {
        movingBody->ApplyForce(0, -jumpForce * 9.8);
        lastJump = movingBody->currentTime;
    }
}

bool Movements::CanJump()
{
    if (movingBody->currentTime - lastJump > jumpTimer)
    {
        return true;
    }
    return false;
}

bool Movements::IsOnGround()
{
    const float groundTolerance = 0.01f;
    float objectBottomY = movingBody->body.pos.y + movingBody->body.radius * 2;
    float groundY = movingBody->screenHeight - movingBody->taskbarHeight;

    bool isOnGround = objectBottomY >= (groundY - groundTolerance);

    return isOnGround;
}

void Movements::RunFrom(POINT target, float speed)
{
    // Calculate the direction vector (reversed from MoveTo)
    float dx = (movingBody->body.pos.x + movingBody->body.radius) - target.x;
    float dy = (movingBody->body.pos.y + movingBody->body.radius) - target.y;

    // Calculate the distance from the target
    float distance = std::sqrt(dx * dx + dy * dy);

    // Define a "safe" distance
    float safeDistance = movingBody->body.radius * 10; // Adjust this value as needed

    if (distance < safeDistance) // If too close to the target
    {
        // Normalize the direction
        dx /= distance;
        dy /= distance;

        // Calculate desired velocity (away from target)
        float desiredVx = dx * speed;
        float desiredVy = dy * speed;

        // Calculate steering force
        float steeringX = desiredVx - movingBody->body.velocity.x;
        float steeringY = desiredVy - movingBody->body.velocity.y;

        // Apply steering force
        movingBody->ApplyForce(steeringX, steeringY);
    }
    else // Far enough from target, start slowing down
    {
        // Apply braking force
        float brakingForce = speed * 2;
        float currentSpeed = std::sqrt(movingBody->body.velocity.x * movingBody->body.velocity.x +
            movingBody->body.velocity.y * movingBody->body.velocity.y);

        if (currentSpeed > 0.1f)
        {
            float fx = -movingBody->body.velocity.x / currentSpeed * brakingForce;
            float fy = -movingBody->body.velocity.y / currentSpeed * brakingForce;
            movingBody->ApplyForce(fx, fy);
        }
    }
}


void Movements::MovingMode(bool isClimbing)
{
}
