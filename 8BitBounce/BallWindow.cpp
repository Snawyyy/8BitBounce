#include "Ball.h"

// Global variables
int width = 50;
int height = 50;
int centerW = width / 2;
int centerH = height / 2;

float ballX = centerW;
float ballY = centerH;
float ballVx = 20; // Initial horizontal velocity
float ballVy = 0; // Initial vertical velocity
float ballAccY = 2; // Gravity (vertical acceleration)
float ballRadius = width / 2;
float friction = 0.99;

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

// Timer ID
const int TIMER_ID = 1;

LRESULT CALLBACK BallWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	{
		static HBRUSH hBrush = NULL;
		static RECT rect;
		PAINTSTRUCT ps;
		HDC hdc;
		static bool isPressed;

		switch (uMsg)
		{
		case WM_CREATE:
		{
			// Creates a solid brush for the button background color
			hBrush = CreateSolidBrush(RGB(100, 0, 0)); // Change the RGB values to set your desired background color

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
				// Update the ball's position
				ballX += ballVx;
				ballY += ballVy;

				// Update the ball's velocity (apply gravity)
				ballVy += ballAccY;

				ballVx = ballVx * friction;
				ballVy = ballVy * friction;

				// Collision with top and bottom of screen
				if (ballX + ballRadius > screenWidth)
				{
					ballVx = -ballVx;
				}
				if (ballX + ballRadius < 0)
				{
					ballVx = -ballVx;
				}
				// collision with left and right of screen
				if (ballY + ballRadius > screenHeight)
				{
					ballVy = -ballVy;
				}
				if (ballY + ballRadius < 0)
				{
					ballVy = -ballVy;
				}

				SetWindowPos(hWnd, NULL, ballX, ballY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				// Redraw the ball
				InvalidateRect(hWnd, NULL, TRUE);
			}
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
		case WM_NCHITTEST: // Window Dragging logic
		{
			// Convert the mouse position to screen coordinates
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			ScreenToClient(hWnd, &pt);

			// Define the draggable area, e.g., top 50 pixels of the window
			RECT draggableArea = { 0, 0, width, height }; // You need to define windowWidth

			// Check if the point is within the draggable area
			if (PtInRect(&draggableArea, pt))
			{
				return HTCAPTION;
			}
			else
			{
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
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