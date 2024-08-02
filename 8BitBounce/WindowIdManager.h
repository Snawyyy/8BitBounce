#pragma once

#include <windows.h>
#include <unordered_map>

class WindowIdManager {
private:
    std::unordered_map<HWND, int> handleToIdMap;
    int nextId;

    WindowIdManager();  // Private constructor
    WindowIdManager(const WindowIdManager&) = delete;  // Disable copy constructor
    WindowIdManager& operator=(const WindowIdManager&) = delete;  // Disable assignment operator

public:

    int objectsCount;

    static WindowIdManager& getInstance();
    int getWindowId(HWND hwnd);
};