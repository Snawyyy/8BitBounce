#include "DropDownOptions.h"

LRESULT CALLBACK DropDownOptionsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush = NULL;
	static RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	WindowPhysics* pBall = reinterpret_cast<WindowPhysics*>(GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = (rect.bottom - rect.top) / 6;

		HWND option1 = CreateWindowA("BUTTON",
			"option1",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, 0, width, height,
			hWnd, (HMENU)2, NULL, NULL);

		HWND option2 = CreateWindowA("BUTTON",
			"option2",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, height, width, height,
			hWnd, (HMENU)3, NULL, NULL);

		HWND option3 = CreateWindowA("BUTTON",
			"option3",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, height * 2, width, height,
			hWnd, (HMENU)4, NULL, NULL);

		HWND option4 = CreateWindowA("BUTTON",
			"option4",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, height * 3, width, height,
			hWnd, (HMENU)4, NULL, NULL);

		HWND option5 = CreateWindowA("BUTTON",
			"option5",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, height * 4, width, height,
			hWnd, (HMENU)4, NULL, NULL);

		HWND option6 = CreateWindowA("BUTTON",
			"option6",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, height * 5, width, height,
			hWnd, (HMENU)4, NULL, NULL);

		SetFocus(hWnd);
		break;
	}
	case WM_DRAWITEM:
	{
		Button option1(lParam, 2, L"Gravity");
		option1.Draw(RGB(255, 255, 255), RGB(1, 0, 0));

		Button option2(lParam, 3, L"Add Mass");
		option2.Draw(RGB(255, 255, 255), RGB(1, 0, 0));

		Button option3(lParam, 4, L"Reduce Mass");
		option3.Draw(RGB(255, 255, 255), RGB(1, 0, 0));

		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FrameRect(hdc, &rect, hBrush);
		FillRect(hdc, &rect, hBrush);
		EndPaint(hWnd, &ps);

		break;
	}
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE) 
		{
			SendMessage(GetParent(hWnd), WM_USER + 2, 0, 0);

			DestroyWindow(hWnd);
		}
		break;
	}
	case WM_DESTROY:
	{
		// Clean up resources
		DeleteObject(hBrush);
		DestroyWindow(hWnd);
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void DropDownOptions(HWND parent)
{
	HINSTANCE m_hinstance = GetModuleHandle(nullptr);
	const wchar_t* CLASS_NAME = L"DropDownOptionsClass";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hinstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = DropDownOptionsProc;
	wndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	RegisterClass(&wndClass);

	DWORD style = WS_POPUP | WS_CHILD;

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	int height = GetSystemMetrics(SM_CYSCREEN) / 6;
	int width = height / 2;

	HWND hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		CLASS_NAME,
		L"Ball",
		style,
		cursorPos.x,
		cursorPos.y,
		width,
		height,
		parent,
		NULL,
		m_hinstance,
		NULL
	);

	ShowWindow(hWnd, SW_SHOW);

}