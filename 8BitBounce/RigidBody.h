#pragma once
#include "Physics.h"
#include <windows.h>

struct Vector2
{
    float x;
    float y;
};

class RigidBody : public Physics
{
private:
    bool isGrounded = false;
    bool isDragging = false;
    POINT clickOffset;

    // Previous body x,y coords
    float preBodyX;
    float preBodyY;

    // velocity
    float bodyVx = 0; // Initial horizontal velocity
    float bodyVy = 0; // Initial vertical velocity

    float gravity = 9.8; // Gravity (vertical acceleration)
    float friction = 0.4f;
    float staticFriction = 0.5f;
    float dampingFactor = 0.9f; // damping factor for energy loss on bounce
    float restitution = 0.9; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)
    const double G = 6.67430e-11;  // gravitational constant in m^3 kg^-1 s^-2
    const double EARTH_MASS = 5.9722e24;  // mass of the Earth in kg
    const double EARTH_RADIUS = 6.371e6;  // radius of the Earth in meters

    void CalculateTime();
    void UpdatePosition();
    void ApplyWorldGravity();
    void ApplyGravity(double m1, double m2, double r);
    void ApplyFriction();
    void BorderCollisions();
    void Draggable();
    int GetTaskbarHeight();

public:
    // Body x,y coords
    float bodyX;
    float bodyY;
    float mass = 1.0f; // Mass of the rigid body
    Vector2 force = { 0.0f, 0.0f };

    RigidBody(HWND windowHandle);
    void RunPhysics() override;
    void ApplyForce(float addForceX, float addForceY);
    void Grab();
    void TrackGrabbing();
    void Ungrab();    
    void CalculateCollisions();

    int Clamp(int num, int min, int max);

};
