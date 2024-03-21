#include "ControllerWindow.h"

LRESULT CALLBACK ControllerWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    int width = rcClient.right - rcClient.left;
    int height = rcClient.bottom - rcClient.top;

    switch (uMsg)
    {
    case WM_CREATE: // where you create all the interface
    {
        // Window Ui buttons
        // 
        // Send Button
        HWND sendButton = CreateWindowA("BUTTON",
            "Ball",
            WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            width / 2 - BUTTON_WIDTH / 2, height / 2 - BUTTON_HEIGHT / 2 + BAR_MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT,
            hWnd, (HMENU)SEND_ID, NULL, NULL);

        // Window Bar Buttons
        // 
        // Quit Button
        HWND quitButton = CreateWindowA("BUTTON",
            "Quit",
            WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            (width - BAR_BUTTON_SIZE - BAR_MARGIN), BAR_MARGIN, BAR_BUTTON_SIZE, BAR_BUTTON_SIZE,
            hWnd, (HMENU)QUIT_BUTTON_ID, NULL, NULL);

        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    }
    case WM_COMMAND: // Button logic
    {

        switch (LOWORD(wParam))
        {
        case QUIT_BUTTON_ID: // Knows what button number was pressed
        {
            PostQuitMessage(0);
            break;
        }
        case SEND_ID:
        {
            CreateBallWindow(NULL, GetModuleHandle(nullptr), 0, 0, 0, 0);
            break;
        }

        break;
        }

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);

        WindowFrame(hdc, hWnd, width, height);
        WindowBar(hdc, hWnd, width);

        // Cleanup
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DRAWITEM:
    {
        QuitButton(lParam, QUIT_BUTTON_ID);
        DefaultButton(lParam, L"Ball", SEND_ID);
        break;
    }
    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, CreateSolidBrush(BACKGROUND_COLOR)); // Fill with red
        return TRUE; // Return non-zero value when processed
    }
    case WM_NCHITTEST: // Window Dragging logic
    {
        // Convert the mouse position to screen coordinates
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        ScreenToClient(hWnd, &pt);

        // Define the draggable area, e.g., top 50 pixels of the window
        RECT draggableArea = { 0, 0, width, WIN_BAR_SIZE }; // You need to define windowWidth

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
    case WM_CLOSE: {
        DestroyWindow(hWnd);
        return 0;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window() : m_hinstance(GetModuleHandle(nullptr)) 
{
    const wchar_t* CLASS_NAME = L"Snawy's Window Class";

    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hinstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = ControllerWindowProc;

    wndClass.hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);
    RegisterClass(&wndClass);

    // LetterWindow Class
    WNDCLASS wc = {};
    wc.lpfnWndProc = BallWindowProc;
    wc.hInstance = m_hinstance;
    wc.lpszClassName = L"ChildWindowClass";

    RegisterClass(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int width = screenWidth / 10;
    int height = screenWidth / 12;


    int x = (screenWidth / 2) - width / 2;
    int y = (screenHeight / 2) - height / 2;

    // Get the dimensions of the parent window
    RECT parentRect;
    // Create the window.
    m_hWnd = CreateWindowEx(
        0,                 // Optional window styles.
        CLASS_NAME,        // Window class
        L"Controller Window",   // Window text
        WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN, // Window style
        x, y, width, height,   // Size and position
        NULL,           // Parent window    
        NULL,              // Menu
        m_hinstance,         // Instance handle
        NULL  // Additional application data
    );
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