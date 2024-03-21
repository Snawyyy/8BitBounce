#include "BallWindow.h"

// Global variables

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int taskbarHeight = GetTaskbarHeight();

int width = screenWidth / 30;
int height = screenWidth / 30;

int centerW = screenWidth / 2;
int centerH = screenHeight / 2;

// Ball x,y cords
float ballX = centerW;
float ballY = centerH;

// velocity
float ballVx = 20; // Initial horizontal velocity
float ballVy = 0; // Initial vertical velocity

// Previous ball x,y cords
float preBallx;
float preBally;

float ballAccY = 1.5; // Gravity (vertical acceleration)
float ballRadius = width / 2;
float friction = 0.99f;
float dampingFactor = 0.85f; // damping factor for energy loss on bounce.
float restitution = 0.9; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)

// Delta time calculations variables

DWORD lastTime = 0;
DWORD currentTime = 0;
float deltaTime = 0;


// Timer ID
const int TIMER_ID = 1;

bool isDragging = false;

LRESULT CALLBACK BallWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	{
		static HBRUSH hBrush = NULL;
		static RECT rect;
		PAINTSTRUCT ps;
		HDC hdc;

		switch (uMsg)
		{
		case WM_CREATE:
		{
			// Creates a solid brush for the button background color
			hBrush = CreateSolidBrush(RGB(100, 0, 0)); // Change the RGB values to set your desired background color
			ballX = centerW;
			preBallx = centerW;

			lastTime = GetTickCount();

			SetTimer(hWnd, TIMER_ID, 16, NULL); // 16ms interval (approximately 60 FPS)


			break;
		}
		case WM_SIZE:
		{
			// Stores the button's client area dimensions
			GetClientRect(hWnd, &rect);

			// Create a circular region based on the updated client rect
			HRGN hRgn = CreateEllipticRgn(0, 0, rect.right, rect.bottom);
			SetWindowRgn(hWnd, hRgn, TRUE); // Set the window region. TRUE to redraw the window immediately

			// NOTE: The region is now owned by the system, no need to call DeleteObject on hRgn
			break;
		}
		case WM_TIMER:
		{
			if (wParam == TIMER_ID)
			{

				currentTime = GetTickCount();
				deltaTime = (currentTime - lastTime) / 20.0f;
				lastTime = currentTime;

				preBallx = ballX;
				preBally = ballY;

				// Update the ball's position
				ballX += (ballVx * deltaTime);
				ballY += (ballVy * deltaTime);

				bool isGrounded = ballY + height >= screenHeight - taskbarHeight - 1;
				if (!isGrounded)
				{
					ballVy += (ballAccY * deltaTime);
				}
				else
				{
					if ((ballVy < 10 && ballVy > -10))
					{
						ballVy = (ballVy * friction / (10 / ballVy));
					}
					ballVx *= friction;
				}

				// Apply friction more effectively
				ballVx *= friction;
				ballVy *= friction;


				// Collision with left and right of screen
				if (ballX + width > screenWidth)
				{
					ballVx = -ballVx * dampingFactor * restitution; // Apply friction and restitution on bounce
					ballX = screenWidth - width; // Prevents the ball from getting stuck right to the screen
				}
				if (ballX < 0)
				{
					ballVx = -ballVx * dampingFactor * restitution; // Apply friction and restitution on bounce
					ballX = 0; // Prevents the ball from getting stuck left to the screen
				}

				// Collision with top and bottom of screen
				if (ballY + height > screenHeight - taskbarHeight)
				{
					ballVy = -(ballVy * dampingFactor * restitution);
					ballY = screenHeight - taskbarHeight - height + 1; // Prevents the ball from getting stuck below the screen
				}
				if (ballY < 0)
				{
					ballVy = -ballVy * dampingFactor * restitution;
					ballY = 0; // Prevents the ball from getting stuck above the screen
				}

				POINT cursorPos;
				GetCursorPos(&cursorPos);
				if (isDragging)
				{
					ballX = cursorPos.x - ballRadius;
					ballY = cursorPos.y - ballRadius;
					// Optionally reset velocities to 0 or apply some logic to handle dragging behavior
				}

				SetWindowPos(hWnd, NULL, ballX, ballY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				// InvalidateRect(hWnd, NULL, TRUE); // If needed, force the window to redraw
			}
			break;
		}

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			// Create the outer and inner rectangles
			RECT outerRect = { 0, 0, rect.right, rect.bottom };
			RECT innerRect = { 5, 5, rect.right - 5, rect.bottom - 5 };

			// Create the outer and inner regions
			HRGN outerRgn = CreateEllipticRgnIndirect(&outerRect);
			HRGN innerRgn = CreateEllipticRgnIndirect(&innerRect);

			// Combine the regions to create a ring shape
			CombineRgn(outerRgn, outerRgn, innerRgn, RGN_DIFF);
			// Set the brush color for the outer ring
			HBRUSH outerBrush = CreateSolidBrush(RGB(100,0,0));
			SelectObject(hdc, outerBrush);

			// Fill the outer ring
			FillRgn(hdc, outerRgn, outerBrush);

			// Set the brush color for the inner circle
			HBRUSH innerBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, innerBrush);

			// Fill the inner circle
			Ellipse(hdc, innerRect.left, innerRect.top, innerRect.right, innerRect.bottom);
			FillRgn(hdc, innerRgn, innerBrush);
			// Clean up the brushes and regions
			DeleteObject(outerBrush);
			DeleteObject(innerBrush);
			DeleteObject(outerRgn);
			DeleteObject(innerRgn);

			EndPaint(hWnd, &ps);

			break;
		}
		case WM_MOUSEMOVE:
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE; // Specifies that we want a WM_MOUSELEAVE message when the mouse leaves
			tme.hwndTrack = hWnd;
			tme.dwHoverTime = HOVER_DEFAULT; // Not needed for WM_MOUSELEAVE but required to be set

			TrackMouseEvent(&tme); // Call this function to start tracking

			break;
		}
		case WM_LBUTTONDOWN:
			{
			POINT cursorPos;
			GetCursorPos(&cursorPos);

			// Update the button's pressed state
			isDragging = TRUE;
			ballVx = 0;
			ballVy = 0;
			ballAccY = 0;

			// Set the capture to the window
			SetCapture(hWnd);

			break;
			}
		case WM_LBUTTONUP:
			{
			// Update the button's pressed state
			isDragging = FALSE;
			ballAccY = 2;

			// Calculates the velocity on release aka isDraggin = false
			ballVx = ballX - preBallx;
			ballVy = ballY - preBally;

			// Release the capture
			ReleaseCapture();

			break;
		}
		case WM_DESTROY:
		{
			// Clean up resources
			DeleteObject(hBrush);
			break;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return 0;
	}
}

Window::Window() : m_hinstance(GetModuleHandle(nullptr))
{

	const wchar_t* CLASS_NAME = L"Snawy's Window Class";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hinstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = BallWindowProc;
	wndClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	RegisterClass(&wndClass);

	DWORD style = WS_POPUP | WS_CLIPCHILDREN;


	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Ball",
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		m_hinstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW);

}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Snawy's Window Class";

	UnregisterClass(CLASS_NAME, m_hinstance);
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return true;
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
