#include "WindowManager.h"

std::vector<WindowInfo> windowList;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd)) {
        // Check if the window is not maximized
        WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
        GetWindowPlacement(hwnd, &wp);

        HWND parent = GetParent(hwnd);
        LONG style = GetWindowLong(hwnd, GWL_STYLE);

        if ((parent == NULL || parent == GetDesktopWindow()) &&
            !(style & WS_POPUP) &&
            wp.showCmd != SW_MAXIMIZE && wp.showCmd != SW_MINIMIZE)
        {
            WindowInfo wi;

            wi.hwnd = hwnd;
            GetWindowRect(hwnd, &wi.rect);

            POINT pt = {
                wi.rect.left + (wi.rect.right - wi.rect.left) / 2,
                wi.rect.top + (wi.rect.bottom - wi.rect.top) / 2
            };
            ClientToScreen(hwnd, &pt);

            windowList.push_back(wi);
        }
    }
    return TRUE;
}

void ScanAllWindows()
{
    windowList.clear();
    EnumWindows(EnumWindowsProc, 0);
}

std::vector<WindowInfo> GetWindowsInfo()
{
    ScanAllWindows();

    std::vector<WindowInfo> rects;

    for (const auto& wi : windowList)
    {
        rects.push_back(wi);
    }

    return rects;
}