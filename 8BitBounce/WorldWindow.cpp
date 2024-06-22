#include "WorldWindow.h"

void CreateWindowThread() {
	Window* pWindow = new Window(100, 100);
	pWindow->Show();
	pWindow->ProcessMessages();
	delete pWindow;
}

LRESULT CALLBACK WorldWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush = NULL;
	static RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;


	// Delta time calculations variables
	DWORD lastTime = 0;
	DWORD currentTime = 0;
	float deltaTime = 0;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		std::thread windowThread(CreateWindowThread);
		windowThread.detach();

		std::thread awindowThread(CreateWindowThread);
		awindowThread.detach();


		HWND button = CreateWindowA("BUTTON",
			"Test",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, 0, BUTTON_HEIGHT, BUTTON_HEIGHT,
			hWnd, (HMENU)QUIT_BUTTON_ID, NULL, NULL);

		SetTimer(hWnd, 2, 8, NULL); // 16ms interval (approximately 60 FPS)

		break;
	}
	case WM_TIMER:
	{
		if (wParam == 2)
		{
			currentTime = GetTickCount64();
			deltaTime = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;
		}
		break;
	}
	case WM_DRAWITEM:
	{
		Button quitButton(lParam, QUIT_BUTTON_ID, L"X");
		quitButton.Draw(RGB(255, 0, 0), RGB(1, 0, 0));

		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		/*
		int newLeft = (SCREEN_WIDTH - (rect.right - rect.left)) / 2;
		int newTop = (SCREEN_HEIGHT - (rect.bottom - rect.top)) / 2;

		rect.left = newLeft;
		rect.top = newTop;
		rect.right = rect.left + 10;
		rect.bottom = rect.top + 10;
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		FrameRect(hdc, &rect, hBrush);
		FillRect(hdc, &rect, hBrush);
		*/

		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);

		break;
	}
	case WM_COMMAND: // Button logic
	{
		switch (LOWORD(wParam))
		{
		case QUIT_BUTTON_ID: // Knows what button number was pressed
			PostQuitMessage(0);
			break;
		}
	}
	case WM_MOUSEMOVE:
	{
		break;
	}
	case WM_LBUTTONDOWN:
	{
		break;
	}
	case WM_LBUTTONUP:
	{
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

WorldWindow::WorldWindow() : m_hinstance(GetModuleHandle(nullptr))
{
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	const wchar_t* CLASS_NAME = L"World_Window";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hinstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WorldWindowProc;
	wndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	RegisterClass(&wndClass);

	DWORD style = WS_POPUP | WS_CLIPCHILDREN;


	m_hWnd = CreateWindowEx(
		WS_EX_LAYERED| WS_EX_TOPMOST,
		CLASS_NAME,
		L"World",
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

	SetLayeredWindowAttributes(m_hWnd, RGB(0,0,0), (255 * 30) / 100, LWA_COLORKEY);
	ShowWindow(m_hWnd, SW_SHOW);

}

WorldWindow::~WorldWindow()
{
	const wchar_t* CLASS_NAME = L"Snawy's Window Class";

	UnregisterClass(CLASS_NAME, m_hinstance);
}

bool WorldWindow::ProcessMessages()
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
