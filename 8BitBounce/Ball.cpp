#include "Ball.h"


int width = 100;
int height = 100;
int centerW = width / 2;
int centerH = height / 2;

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
			if (isPressed)
			{
				// Set the brush color for the outer ring
				HBRUSH outerBrush = CreateSolidBrush(RGB(100, 0, 0));
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
			}
			else
			{
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
			}
			DeleteObject(outerRgn);
			DeleteObject(innerRgn);

			EndPaint(hWnd, &ps);
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