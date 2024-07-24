#include "DropDownOptions.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DropDownOptions(HWND parent, WindowPhysics &rigidBody)
{
    WindowPhysics* s_rigidBody = &rigidBody; // Store the reference to GameObject

    HWND dropDownWindowHandle = NULL;
    const wchar_t CLASS_NAME[] = L"DropDownOptionsClass";

    // Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    int windowWidth = SCREEN_WIDTH / 10;
    int windowHeight = BUTTON_HEIGHT * 5;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    // Create the window
    dropDownWindowHandle = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW, // Always on top and without a taskbar button
        CLASS_NAME,
        L"Drop Down Options",
        WS_POPUP, // Borderless window
        cursorPos.x, cursorPos.y, windowWidth, windowHeight,
        parent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (dropDownWindowHandle == NULL)
    {
        return;
    }

    SetWindowLongPtr(dropDownWindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(s_rigidBody));

    // Show the window
    ShowWindow(dropDownWindowHandle, SW_SHOW);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowPhysics* s_rigidBody = reinterpret_cast<WindowPhysics*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_CREATE:
    {
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int buttonWidth = clientRect.right - clientRect.left;

        int yOffset = 0;

        HWND isGravityButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            0, yOffset, buttonWidth, BUTTON_HEIGHT, hWnd, (HMENU)1, NULL, NULL);
        yOffset += BUTTON_HEIGHT;

        HWND plusMassButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            0, yOffset, buttonWidth, BUTTON_HEIGHT, hWnd, (HMENU)2, NULL, NULL);
        yOffset += BUTTON_HEIGHT;

        HWND minusMassButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            0, yOffset, buttonWidth, BUTTON_HEIGHT, hWnd, (HMENU)3, NULL, NULL);
        yOffset += BUTTON_HEIGHT;

        HWND plusRestitutionButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            0, yOffset, buttonWidth, BUTTON_HEIGHT, hWnd, (HMENU)4, NULL, NULL);
        yOffset += BUTTON_HEIGHT;

        HWND minusRestitutionButton = CreateWindowA("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            0, yOffset, buttonWidth, BUTTON_HEIGHT, hWnd, (HMENU)5, NULL, NULL);
    }
    case WM_DRAWITEM:
    {
        Button isGravity = Button(lParam, 1, L"Gravity");
        isGravity.Draw(DEFULT_BUTTON_COLOR, RGB(0, 0, 1));

        Button plusMass = Button(lParam, 2, L"+Mass");
        plusMass.Draw(DEFULT_BUTTON_COLOR, RGB(0, 0, 1));

        Button minusMass = Button(lParam, 3, L"-Mass");
        minusMass.Draw(DEFULT_BUTTON_COLOR, RGB(0, 0, 1));

        Button plusRestitution = Button(lParam, 4, L"+Restitution");
        plusRestitution.Draw(DEFULT_BUTTON_COLOR, RGB(0, 0, 1));

        Button minusRestitution = Button(lParam, 5, L"-Restitution");
        minusRestitution.Draw(DEFULT_BUTTON_COLOR, RGB(0, 0, 1));
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 1: // isGravity button
        {
            s_rigidBody->worldGravity = !s_rigidBody->worldGravity;
            break;
        }
        case 2: // plusMass button
        {
            s_rigidBody->body.mass = s_rigidBody->body.mass * 2;
            break;
        }
        case 3: // minusMass button
        {
            s_rigidBody->body.mass = s_rigidBody->body.mass / 2;
            break;
        }
        case 4: // plusRestitution button
        {
            if (s_rigidBody->body.restitution < 1)
            {
                s_rigidBody->body.restitution = s_rigidBody->body.restitution + 0.1;
            }
            break;
        }
        case 5: // minusRestitution button
        {
            if (s_rigidBody->body.restitution > 0)
            {
                s_rigidBody->body.restitution = s_rigidBody->body.restitution - 0.1;
            }
            break;
        }
        }
        break;
    }
    case WM_ACTIVATE:
    {
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            SendMessage(GetParent(hWnd), WM_USER + 2, 0, 0);
            DestroyWindow(hWnd);
            return 0;
        }
        break;
    }
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        return 0;
    }
    case WM_DESTROY:
    {
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
