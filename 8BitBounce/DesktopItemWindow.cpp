#include "DesktopItemWindow.h"

struct WindowData
{
    DesktopItemWindow* pWindow;
};

DesktopItemWindow::DesktopItemWindow(HINSTANCE hInstance, int nCmdShow, Creature creatureType, HBITMAP hBitmap)
    : hInstance(hInstance), hBitmap(hBitmap), creature(creatureType), pDragLineWindow(nullptr)
{
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    int width = bitmap.bmWidth;
    int height = bitmap.bmHeight;

    CreateTransparentWindow(width, height);
    SetWindowTransparency();

    Show();
    UpdateWindow(hWnd);
}

DesktopItemWindow::~DesktopItemWindow()
{
    if (pDragLineWindow)
    {
        delete pDragLineWindow;
        pDragLineWindow = nullptr;
    }

    if (hWnd)
    {
        DestroyWindow(hWnd);
    }
}

void DesktopItemWindow::CreateTransparentWindow(int width, int height)
{
    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
        wc.lpszClassName = L"DesktopItemWindowClass";

        if (!RegisterClass(&wc))
        {
            // Handle error
            return;
        }
        classRegistered = true;
    }

    hWnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        L"DesktopItemWindowClass",
        L"",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr, hInstance, this);

    if (!hWnd)
    {
        // Handle error
        return;
    }
}

void DesktopItemWindow::SetWindowTransparency()
{
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY); // Black as transparent color
}

LRESULT CALLBACK DesktopItemWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    const int TIMER_ID = 1;

    WindowData* pData = reinterpret_cast<WindowData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (uMsg == WM_CREATE)
    {

        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        DesktopItemWindow* pWindow = reinterpret_cast<DesktopItemWindow*>(pCreate->lpCreateParams);

        pData = new WindowData;
        pData->pWindow = pWindow;

        pData->pWindow->pPhysics = new WindowPhysics(hWnd);

        RECT windowSize = {};
        GetWindowRect(hWnd, &windowSize);
        pWindow->pPhysics->body.radius = (windowSize.right - windowSize.left) / 2;
        pWindow->pPhysics->lastTime = GetTickCount64();

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pData));
        SetTimer(hWnd, TIMER_ID, 8, NULL);

        pWindow->pPhysics->Grab();
        pWindow->pPhysics->Ungrab();
        return 0;
    }

    if (!pData)
        return DefWindowProc(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_TIMER:
    {
        if (wParam == TIMER_ID)
        {
            pData->pWindow->pPhysics->RunPhysics();
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, pData->pWindow->hBitmap);

        BITMAP bitmap;
        GetObject(pData->pWindow->hBitmap, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        pData->pWindow->pPhysics->Grab();

        // Get the cursor position
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        // Create the DragLineWindow
        if (!pData->pWindow->pDragLineWindow)
        {
            pData->pWindow->pDragLineWindow = new DragLineWindow(pData->pWindow->hInstance);
        }

        SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        pData->pWindow->pDragLineWindow->Show();

        pData->pWindow->pDragLineWindow->UpdatePosition(cursorPos, cursorPos);

        break;
    }

    case WM_MOUSEMOVE:
    {
        pData->pWindow->pPhysics->TrackGrabbing();

        if (pData->pWindow->pPhysics->isDragging)
        {
            // Get the cursor position
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            // Update the DragLineWindow position
            if (pData->pWindow->pDragLineWindow)
            {
                // Calculate the start point based on the window's current position and click offset
                POINT startPoint;
                startPoint.x = static_cast<LONG>(pData->pWindow->pPhysics->body.pos.x + pData->pWindow->pPhysics->clickOffset.x);
                startPoint.y = static_cast<LONG>(pData->pWindow->pPhysics->body.pos.y + pData->pWindow->pPhysics->clickOffset.y);

                pData->pWindow->pDragLineWindow->UpdatePosition(startPoint, cursorPos);
            }
        }

        break;
    }
    case WM_LBUTTONUP:
    {
        pData->pWindow->pPhysics->Ungrab();

        // Hide and destroy the DragLineWindow
        if (pData->pWindow->pDragLineWindow)
        {
            pData->pWindow->pDragLineWindow->Hide();
            delete pData->pWindow->pDragLineWindow;
            pData->pWindow->pDragLineWindow = nullptr;
        }
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        break;
    }
    case WM_USER + 2:
    {
        SetTimer(hWnd, TIMER_ID, 8, NULL);
        pData->pWindow->pPhysics->lastTime = GetTickCount64();
        break;
    }
    case WM_RBUTTONDOWN:
    {
        KillTimer(hWnd, TIMER_ID);
        DropDownOptions(hWnd, *(pData->pWindow->pPhysics));
        break;
    }
    case WM_DESTROY:
    {
        KillTimer(hWnd, TIMER_ID);
        delete pData->pWindow->pPhysics;
        delete pData;
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

void DesktopItemWindow::ItemProcessMessages()
{
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void DesktopItemWindow::Show()
{
    ShowWindow(hWnd, SW_SHOW);
}


void DesktopItemWindow::Hide()
{
    ShowWindow(hWnd, SW_HIDE);
}