#include "WorldWindow.h"

void CreateWindowThread() {
	HBITMAP hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));;
	if (!hBitmap) {
		MessageBox(nullptr, L"Failed to load bitmap", L"Error", MB_ICONERROR);
		return;
	}

	Creature creature = nullptr;

	HINSTANCE hInstance = GetModuleHandle(nullptr);
	if (!hInstance) {
		MessageBox(nullptr, L"Failed to get module handle", L"Error", MB_ICONERROR);
		DeleteObject(hBitmap);
		return;
	}

	int nCmdShow = SW_SHOW;

	try {
		DesktopItemWindow window(hInstance, nCmdShow, creature, hBitmap);
		window.ItemProcessMessages();
	}
	catch (...) {
		MessageBox(nullptr, L"Failed to create DesktopItemWindow", L"Error", MB_ICONERROR);
	}

	DeleteObject(hBitmap);
}

struct DragInfo
{
	bool isDragging;
	POINT startPoint;
	POINT currentPoint;
	HWND draggingWindow;
};

static DragInfo dragInfo = { false, {0, 0}, {0, 0}, NULL };

LRESULT CALLBACK WorldWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush = NULL;
	static RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;

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
		if (dragInfo.isDragging)
		{
			// Convert screen coordinates to client coordinates
			POINT startPointClient = dragInfo.startPoint;
			POINT currentPointClient = dragInfo.currentPoint;

			ScreenToClient(hWnd, &startPointClient);
			ScreenToClient(hWnd, &currentPointClient);

			// Set up the pen
			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // Green line
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

			// Draw the line
			MoveToEx(hdc, startPointClient.x, startPointClient.y, NULL);
			LineTo(hdc, currentPointClient.x, currentPointClient.y);

			// Clean up
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}

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
	case WM_USER_START_DRAG:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		dragInfo.isDragging = true;
		dragInfo.startPoint.x = x;
		dragInfo.startPoint.y = y;
		dragInfo.currentPoint = dragInfo.startPoint;
		dragInfo.draggingWindow = (HWND)wParam;

		InvalidateRect(hWnd, NULL, TRUE);

		break;
	}
	case WM_USER_DRAGGING:
	{
		if (dragInfo.isDragging)
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			dragInfo.currentPoint.x = x;
			dragInfo.currentPoint.y = y;

			InvalidateRect(hWnd, NULL, TRUE);
		}

		break;
	}
	case WM_USER_END_DRAG:
	{
		dragInfo.isDragging = false;
		dragInfo.draggingWindow = NULL;

		InvalidateRect(hWnd, NULL, TRUE);

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

	MemoryManager& worldObjects = MemoryManager::getInstance();
	worldObjects.AllocateMemory(5000);
	worldObjects.InitializeMemory();

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
