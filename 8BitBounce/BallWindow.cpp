#include "BallWindow.h"

// Global variables
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int taskbarHeight = GetTaskbarHeight();

int width = screenWidth / 30;
int height = screenWidth / 30;

int red;
int green;
int blue;

int centerW = screenWidth / 2;
int centerH = screenHeight / 2;
RigidBody ball (nullptr);

// Delta time calculations variables
DWORD lastTime = 0;
DWORD currentTime = 0;
float deltaTime = 0;

// Timer ID
const int TIMER_ID = 1;

LRESULT CALLBACK BallWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	{
		static HBRUSH hBrush = NULL;
		static RECT rect;
		PAINTSTRUCT ps;
		HDC hdc;
		if (!IsWindow(ball.GetWindowHandle()))
		{
			ball.SetWindowHandle(hWnd);
		}

		switch (uMsg)
		{
		case WM_CREATE:
		{
			// Creates a solid brush for the button background color
			hBrush = CreateSolidBrush(RGB(red, green, blue)); // Change the RGB values to set your desired background color

			lastTime = GetTickCount64();

			SetTimer(hWnd, TIMER_ID, 8, NULL); // 16ms interval (approximately 60 FPS)


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
				currentTime = GetTickCount64();
				deltaTime = (currentTime - lastTime) / 1000.0f;
				lastTime = currentTime;

				ball.GetDeltaTime(deltaTime);
				ball.RunPhysics();
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
			HBRUSH innerBrush = CreateSolidBrush(RGB(red, green, blue));
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
			ball.TrackGrabbing();

			break;
		}
		case WM_LBUTTONDOWN:
		{
			ball.Grab();
			break;
		}
		case WM_LBUTTONUP:
		{
			ball.Ungrab();
			break;
		}
		case WM_DESTROY:
		{
			// Clean up resources
			DeleteObject(hBrush);
			KillTimer(hWnd, TIMER_ID);
			PostQuitMessage(0);
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

	DWORD style = WS_POPUP | WS_CHILD;

	HWND worldWindowHandle = FindWindow(NULL, L"World_Window");

	m_hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		CLASS_NAME,
		L"Ball",
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		worldWindowHandle,
		NULL,
		m_hinstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW);

}

void Window::Show()
{
	ShowWindow(m_hWnd, SW_SHOW);
}
Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Snawy's Window Class";

	UnregisterClass(CLASS_NAME, m_hinstance);
}

void Window::ProcessMessages()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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

// Function to change the colors randomly
void changeColorRandomly() {
	red = rand() % 256;   // Generate a random number between 0 and 255
	green = rand() % 256; // Generate a random number between 0 and 255
	blue = rand() % 256;  // Generate a random number between 0 and 255
}
