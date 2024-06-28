#include "WindowIdManager.h"

WindowIdManager::WindowIdManager() : nextId(1) {}

WindowIdManager& WindowIdManager::getInstance() {
    static WindowIdManager instance;
    return instance;
}

int WindowIdManager::getWindowId(HWND hwnd) {
    if (handleToIdMap.find(hwnd) == handleToIdMap.end()) {
        handleToIdMap[hwnd] = nextId++;
    }
    objectsCount = handleToIdMap.size();
    return handleToIdMap[hwnd];
}