#include "DragLineWindow.h"

DragLineWindow::DragLineWindow(HINSTANCE hInstance)
    : hInstance(hInstance), hWnd(NULL), startPoint({ 0, 0 }), endPoint({ 0, 0 })
{
    const wchar_t* CLASS_NAME = L"DragLineWindowClass";

    static bool classRegistered = false;
    if (!classRegistered)
    {
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = DragLineWindow::WindowProc;
        wc.hInstance = hInstance;
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = CLASS_NAME;
        wc.style = CS_HREDRAW | CS_VREDRAW;

        if (!RegisterClass(&wc))
        {
            // Handle error
            return;
        }
        classRegistered = true;
    }

    hWnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"",
        WS_POPUP,
        0, 0, 0, 0,
        nullptr, nullptr, hInstance, this);

    if (!hWnd)
    {
        // Handle error
        return;
    }

    // Set black as the transparent color
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}

DragLineWindow::~DragLineWindow()
{
    if (hWnd)
    {
        DestroyWindow(hWnd);
    }
}

void DragLineWindow::Show()
{
    ShowWindow(hWnd, SW_SHOW);
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void DragLineWindow::Hide()
{
    ShowWindow(hWnd, SW_HIDE);
}

void DragLineWindow::UpdatePosition(POINT startPoint, POINT endPoint)
{
    this->startPoint = startPoint;
    this->endPoint = endPoint;

    // Calculate the rectangle that encompasses the line
    RECT rect;
    rect.left = min(startPoint.x, endPoint.x);
    rect.top = min(startPoint.y, endPoint.y);
    rect.right = max(startPoint.x, endPoint.x);
    rect.bottom = max(startPoint.y, endPoint.y);

    // Add some padding to ensure the line is fully visible
    rect.left -= 1;
    rect.top -= 1;
    rect.right += 1;
    rect.bottom += 1;

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Move and resize the window
    MoveWindow(hWnd, rect.left, rect.top, width, height, TRUE);

    // Adjust the start and end points relative to the window's client area
    this->startPoint.x -= rect.left;
    this->startPoint.y -= rect.top;
    this->endPoint.x -= rect.left;
    this->endPoint.y -= rect.top;

    // Redraw the window
    InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK DragLineWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DragLineWindow* pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        // Retrieve the pointer to the DragLineWindow instance and store it in GWLP_USERDATA
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (DragLineWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else
    {
        // Retrieve the DragLineWindow instance from GWLP_USERDATA
        pThis = (DragLineWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (uMsg)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Set up the pen
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // Green line
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

            // Draw the line
            MoveToEx(hdc, pThis->startPoint.x, pThis->startPoint.y, NULL);
            LineTo(hdc, pThis->endPoint.x, pThis->endPoint.y);

            // Clean up
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);

            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            return 0;
        default:
            break;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
