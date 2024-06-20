#pragma once
#include "Physics.h"
struct Vector2
{
	float x;
	float y;
};

class RigidBody : public Physics
{
private:

	float mass = 1.0f; // Mass of the rigid body
	Vector2 force = { 0.0f, 0.0f };

	bool isGrounded = false;
	bool isDragging = false;
	POINT clickOffset;

	// Previous body x,y cords
	float preBodyX;
	float preBodyY;

	// velocity
	float bodyVx = 0; // Initial horizontal velocity
	float bodyVy = 0; // Initial vertical velocity

	float gravity = 9.8; // Gravity (vertical acceleration)
	float friction = 0.98f;
	float staticFriction = 0.8f;
	float dampingFactor = 0.9f; // damping factor for energy loss on bounce.
	float restitution = 0.9; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)


	void UpdatePosition()
	{
		preBodyX = bodyX;
		preBodyY = bodyY;

		// Calculate acceleration from the applied force
		Vector2 acceleration = { force.x / mass, force.y / mass};

		// Update velocity based on acceleration
		bodyVx += acceleration.x * deltaTime;
		bodyVy += acceleration.y * deltaTime;

		// Update position based on velocity
		bodyX += bodyVx * deltaTime;
		bodyY += bodyVy * deltaTime;

		// Reset the applied force after updating the position
		force = Vector2{ 0.0f, 0.0f };
	}

	void ApplyGravity()
	{
		force.y += gravity * mass * SECOND_TO_MILISECOND;
	}

	void ApplyFriction()
	{
		Vector2 frictionForce = Vector2{ - bodyVx * friction, -bodyVy * friction};
		force.x += frictionForce.x;
		force.y += frictionForce.y;
	}

	void BorderCollisions()
	{
		// Collision with left and right of screen
		if (bodyX + width > screenWidth)
		{
			bodyVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
			bodyX = screenWidth - width; // Prevents the body from getting stuck right to the screen
			force.x -= 2.0f * bodyVx * mass; // Apply impulse force
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
			force.y -= 2.0f * bodyVy * mass; // Apply impulse force
		}
		if (bodyY < 0)
		{
			bodyVy = -(bodyVy * dampingFactor * restitution);
			bodyY = 0; // Prevents the body from getting stuck above the screen
			force.y -= 2.0f * bodyVy * mass; // Apply impulse force
		}
	}

	void Draggable()
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		if (isDragging)
		{
			bodyX = cursorPos.x  - clickOffset.x;
			bodyY = cursorPos.y  - clickOffset.y;
		}
	}

	int GetTaskbarHeight()
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

public:

	// Body x,y cords
	float bodyX = centerW;
	float bodyY = centerH;

	RigidBody(HWND windowHandle):Physics(windowHandle)
	{
		bodyX = centerW;
		preBodyX = centerW;
		preBodyY = centerH;
	}

	void RunPhysics() override
	{
		UpdatePosition();
		ApplyGravity();
		ApplyFriction();
		BorderCollisions();
		Draggable();
	}

	void ApplyForce(float addForceX, float addForceY)
	{
		force.x += addForceX;
		force.y += addForceY;
	}

	void Grab()
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

	void TrackGrabbing()
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE; // Specifies that we want a WM_MOUSELEAVE message when the mouse leaves
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = HOVER_DEFAULT; // Not needed for WM_MOUSELEAVE but required to be set

		TrackMouseEvent(&tme); // Call this function to start tracking
	}

	void Ungrab()
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

};