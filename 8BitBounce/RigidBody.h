#pragma once
#include "Physics.h"

class RigidBody : public Physics
{
private:

	// Body x,y cords
	float bodyX = centerW;
	float bodyY = centerH;

	bool isGrounded = false;
	bool isDragging = false;
	POINT clickOffset;

	// Previous body x,y cords
	float preBodyX;
	float preBodyY;

	// velocity
	float bodyVx = 0; // Initial horizontal velocity
	float bodyVy = 0; // Initial vertical velocity

	float gravity = 3; // Gravity (vertical acceleration)
	float friction = 0.98f;
	float dampingFactor = 0.8f; // damping factor for energy loss on bounce.
	float restitution = 0.9; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)


	void UpdatePosition()
	{
		bodyX += (bodyVx * this->time);
		bodyY += (bodyVy * this->time);
		isGrounded = bodyY + height >= screenHeight - taskbarHeight;
	}

	void ApplyGravity()
	{
		if (!isGrounded)
		{
			bodyVy += (gravity * this->time) * SECOND_TO_MILISECOND;
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

	void applyFriction()
	{
		bodyVx *= friction;
		bodyVy *= friction;
	}

	void BorderCollisions()
	{
		// Collision with left and right of screen
		if (bodyX + width > screenWidth)
		{
			bodyVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
			bodyX = screenWidth - width; // Prevents the body from getting stuck right to the screen
			InvalidateRect(hWnd, NULL, NULL);
			RedrawWindow(hWnd, NULL, 0, 0);
		}
		if (bodyX < 0)
		{
			bodyVx = -(bodyVx * dampingFactor * restitution); // Apply friction and restitution on bounce
			bodyX = 0; // Prevents the body from getting stuck left to the screen
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
			InvalidateRect(hWnd, NULL, NULL);
			RedrawWindow(hWnd, NULL, 0, 0);
		}
	}

	void SetPreCords()
	{
		preBodyX = bodyX;
		preBodyY = bodyY;
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

	RigidBody(HWND windowHandle):Physics(windowHandle)
	{
		bodyX = centerW;
		preBodyX = centerW;
		preBodyY = centerH;
	}

	void RunPhysics() override
	{
		UpdatePosition();
		SetPreCords();
		ApplyGravity();
		applyFriction();
		BorderCollisions();
		Draggable();

		SetWindowPos(hWnd, NULL, bodyX, bodyY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
	
	void Grab()
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		clickOffset.x = cursorPos.x - bodyX;
		clickOffset.y = cursorPos.y - bodyY;

		// Update the button's pressed state
		isDragging = TRUE;
		bodyVx = 0;
		bodyVy = 0;
		gravity = 0;

		// Set the capture to the window
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
		gravity = 3;

		// Calculates the velocity on release aka isDraggin = false
		bodyVx = (bodyX - preBodyX) * 50;
		bodyVy = (bodyY - preBodyY) * 50;

		// Release the capture
		ReleaseCapture();
	}

};