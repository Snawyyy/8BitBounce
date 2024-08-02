#pragma once
#include <vector>
#include "Windows.h"

struct WindowInfo {
    HWND hwnd;
    RECT rect;
};

BOOL EnumWindowsProc(HWND hwnd, LPARAM lParam);

void ScanAllWindows();

std::vector<WindowInfo> GetWindowsInfo();
