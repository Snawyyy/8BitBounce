#pragma once
#include <Windows.h>

#define SECOND_TO_MILISECOND 1000
#define VELOCITY_THRESHOLD 10


class RigidObject
{
private:

	HWND hWnd;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int taskbarHeight = GetTaskbarHeight();
	int centerW = screenWidth / 2;
	int centerH = screenHeight / 2;

	int width;
	int height;
	float deltaTime = 0;

	// Body x,y cords
	float bodyX = centerW;
	float bodyY = centerH;
	bool isGrounded;
	bool isDragging = false;
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

	void UpdatePosition()
	{
		bodyX += (bodyVx * deltaTime);
		bodyY += (bodyVy * deltaTime);
	    isGrounded = bodyY + height >= screenHeight - taskbarHeight - 1;
	}

	void ApplyGravity()
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
			bodyX = cursorPos.x - bodyRadius;
			bodyY = cursorPos.y - bodyRadius;
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

		void RigidBody(HWND windowHandle)
		{
			hWnd = windowHandle;

			bodyX = centerW;
			preBodyX = centerW;

			RECT windowRect;
			GetWindowRect(hWnd, &windowRect);
			width = windowRect.right - windowRect.right;
			height = windowRect.bottom - windowRect.top;
		}

		HWND GetWindowHandle()
		{
			return this->hWnd;
		}

		void RunPhysics()
		{
			UpdatePosition();
			SetPreCords();
			ApplyGravity();
			applyFriction();
			BorderCollisions();
			Draggable();

			SetWindowPos(hWnd, NULL, bodyX, bodyY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		void GetDeltaTime(float dt)
		{
			this->deltaTime = dt;
		}

		void Grab()
		{
			POINT cursorPos;
			GetCursorPos(&cursorPos);

			// Update the button's pressed state
			isDragging = TRUE;
			bodyVx = 0;
			bodyVy = 0;
			gravity = 0;

			// Set the capture to the window
			SetCapture(hWnd);
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