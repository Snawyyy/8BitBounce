#include "BallWindow.h"

// Global variables
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int taskbarHeight = GetTaskbarHeight();

int ballWidth = screenWidth / 30;
int ballHeight = screenWidth / 30;

int centerW = screenWidth / 2;
int centerH = screenHeight / 2;

int red;
int green;
int blue;

// Timer ID
const int TIMER_ID = 1;

LRESULT CALLBACK BallWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Ball* pBall = reinterpret_cast<Ball*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    static HBRUSH hBrush = NULL;
    static RECT rect;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // Creates a solid brush for the button background color
        hBrush = CreateSolidBrush(RGB(red, green, blue));

        // Create a new Ball instance dynamically
        pBall = new Ball();

        // Set the Ball instance pointer as the GWLP_USERDATA value
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pBall));

        // Initialize the Ball instance members
        pBall->x = centerW;
        pBall->prevX = centerW;
        pBall->prevY = centerH;
        pBall->lastTime = GetTickCount();

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
            pBall->currentTime = GetTickCount();
            pBall->deltaTime = (pBall->currentTime - pBall->lastTime) / 20.0f;
            pBall->lastTime = pBall->currentTime;

            pBall->prevX = pBall->x;
            pBall->prevY = pBall->y;

            // Update the ball's position
            pBall->x += (pBall->vx * pBall->deltaTime);
            pBall->y += (pBall->vy * pBall->deltaTime);

            bool isGrounded = pBall->y + ballHeight >= screenHeight - taskbarHeight - 1;
            if (!isGrounded)
            {
                pBall->vy += (pBall->accY * pBall->deltaTime);
            }
            else
            {
                if ((pBall->vy < 10 && pBall->vy > -10))
                {
                    pBall->vy = (pBall->vy * pBall->friction / (10 / pBall->vy));
                }
                pBall->vx *= pBall->friction;
            }

            // Apply friction more effectively
            pBall->vx *= pBall->friction;
            pBall->vy *= pBall->friction;

            // Collision with left and right of screen
            if (pBall->x + ballWidth > screenWidth)
            {
                pBall->vx = -pBall->vx * pBall->dampingFactor * pBall->restitution; // Apply friction and restitution on bounce
                pBall->x = screenWidth - ballWidth; // Prevents the ball from getting stuck right to the screen
                changeColorRandomly();
                InvalidateRect(hWnd, NULL, NULL);
                RedrawWindow(hWnd, NULL, 0, 0);
            }
            if (pBall->x < 0)
            {
                pBall->vx = -pBall->vx * pBall->dampingFactor * pBall->restitution; // Apply friction and restitution on bounce
                pBall->x = 0; // Prevents the ball from getting stuck left to the screen
                changeColorRandomly();
                InvalidateRect(hWnd, NULL, NULL);
                RedrawWindow(hWnd, NULL, 0, 0);
            }

            // Collision with top and bottom of screen
            if (pBall->y + ballHeight > screenHeight - taskbarHeight)
            {
                pBall->vy = -(pBall->vy * pBall->dampingFactor * pBall->restitution);
                pBall->y = screenHeight - taskbarHeight - ballHeight + 1; // Prevents the ball from getting stuck below the screen
            }
            if (pBall->y < 0)
            {
                pBall->vy = -pBall->vy * pBall->dampingFactor * pBall->restitution;
                pBall->y = 0; // Prevents the ball from getting stuck above the screen
                changeColorRandomly();
                InvalidateRect(hWnd, NULL, NULL);
                RedrawWindow(hWnd, NULL, 0, 0);
            }

            POINT cursorPos;
            GetCursorPos(&cursorPos);
            if (pBall->isDragging)
            {
                pBall->x = cursorPos.x - pBall->radius;
                pBall->y = cursorPos.y - pBall->radius;
                // Optionally reset velocities to 0 or apply some logic to handle dragging behavior
            }

            SetWindowPos(hWnd, NULL, pBall->x, pBall->y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
        HBRUSH outerBrush = CreateSolidBrush(RGB(0, 0, 0));
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
        pBall->isDragging = TRUE;
        pBall->vx = 0;
        pBall->vy = 0;
        pBall->accY = 0;

        // Set the capture to the window
        SetCapture(hWnd);

        break;
    }
    case WM_LBUTTONUP:
    {
        // Update the button's pressed state
        pBall->isDragging = FALSE;
        pBall->accY = 2;

        // Calculates the velocity on release aka isDraggin = false
        pBall->vx = pBall->x - pBall->prevX;
        pBall->vy = pBall->y - pBall->prevY;

        // Release the capture
        ReleaseCapture();

        break;
    }
    case WM_DESTROY:
    {
        // Clean up resources
        DeleteObject(hBrush);

        // Delete the dynamically allocated Ball instance
        delete pBall;

        DestroyWindow(hWnd);
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

HWND CreateBallWindow(HWND hParent, HINSTANCE hInstance, int x, int y, int width, int height)
{
    const wchar_t* CLASS_NAME = L"ChildWindowClass";

    DWORD style = WS_POPUP | WS_CLIPCHILDREN | WS_CHILDWINDOW | WS_CLIPSIBLINGS;

    // Create a new Ball instance dynamically
    Ball* pBall = new Ball();

    HWND BallWindow = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Ball",
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        ballWidth,
        ballHeight,
        NULL,
        NULL,
        hInstance,
        NULL // No need to pass the Ball instance pointer here
    );

    // Set the Ball instance pointer as the GWLP_USERDATA value
    SetWindowLongPtr(BallWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pBall));

    ShowWindow(BallWindow, SW_SHOW);

    return BallWindow;
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