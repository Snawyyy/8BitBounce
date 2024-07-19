#pragma once
#include <windows.h>
#include "WindowPhysics.h"
#include "DropDownOptions.h"
#include "Creature.h"

class DesktopItemWindow
{
public:
    Creature creature = nullptr;
    WindowPhysics* pPhysics;

    DesktopItemWindow(HINSTANCE hInstance, int nCmdShow, Creature creatureType, HBITMAP hBitmap);
    ~DesktopItemWindow();

    void ItemProcessMessages();

    void Show();
    void Hide();

private:
    HWND hWnd;
    HINSTANCE hInstance;
    HBITMAP hBitmap;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void CreateTransparentWindow(int width, int height);
    void SetWindowTransparency();
};
