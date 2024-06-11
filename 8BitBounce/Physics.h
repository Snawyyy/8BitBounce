#pragma once
#include <Windows.h>

#define SECOND_TO_MILISECOND 1000
#define VELOCITY_THRESHOLD 10
class RigidObject
{
public:
	HWND hWnd;

	int width;
	int height;

	// Body x,y cords
	float bodyX = centerW;
	float bodyY = centerH;
	bool isGrounded;

	// Previous body x,y cords
	float preBodyX;
	float preBodyY;


	// velocity
	float bodyVx = 0; // Initial horizontal velocity
	float bodyVy = 0; // Initial vertical velocity


	float gravity = 3; // Gravity (vertical acceleration)
	float friction = 0.99f;
	float dampingFactor = 0.85f; // damping factor for energy loss on bounce.
	float restitution = 0.9; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)

	float bodyRadius = width / 2;

	void RigidBody(HWND windowHandle)
	{
		hWnd = windowHandle;
	}

	HWND GetWindowHandle()
	{
		return this->hWnd;
	}

	void SetPreCords()
	{
		preBodyX = bodyX;
		preBodyY = bodyY;
	}

	void UpdatePosition()
	{
		bodyX += (bodyVx * deltaTime);
		bodyY += (bodyVy * deltaTime);
	    isGrounded = bodyY + height >= screenHeight - taskbarHeight - 1;
	}

	void ApplyGravity(float deltaTime)
	{
		if (!isGrounded)
		{
			bodyVy += (gravity * deltaTime) * SECOND_TO_MILISECOND;
		}
		else
		{
			if ((bodyVy < VELOCITY_THRESHOLD && bodyVy > -VELOCITY_THRESHOLD))
			{
				bodyVy = (bodyVy * friction / (VELOCITY_THRESHOLD / bodyVy));
			}
			bodyVx *= friction;
		}
	}

	void BorderCollisions()
	{
		// Collision with left and right of screen
		if (bodyX + width > screenWidth)
		{
			bodyVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
			bodyX = screenWidth - width; // Prevents the body from getting stuck right to the screen
			changeColorRandomly();
			InvalidateRect(hWnd, NULL, NULL);
			RedrawWindow(hWnd, NULL, 0, 0);
		}
		if (bodyX < 0)
		{
			ballVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
			bodyX = 0; // Prevents the body from getting stuck left to the screen
			changeColorRandomly();
			InvalidateRect(hWnd, NULL, NULL);
			RedrawWindow(hWnd, NULL, 0, 0);
		}

		// Collision with top and bottom of screen
		if (bodyY + height > screenHeight - taskbarHeight)
		{
			bodyVy = -(bodyVy * dampingFactor * restitution);
			bodyY = screenHeight - taskbarHeight - height; // Prevents the body from getting stuck below the screen
		}
		if (bodyY < 0)
		{
			bodyVy = -(bodyVy * dampingFactor * restitution);
			bodyY = 0; // Prevents the body from getting stuck above the screen
			changeColorRandomly();
			InvalidateRect(hWnd, NULL, NULL);
			RedrawWindow(hWnd, NULL, 0, 0);
		}
	}

	void Draggable()
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		if (isDragging)
		{
			ballX = cursorPos.x - ballRadius;
			ballY = cursorPos.y - ballRadius;
		}
	}
};