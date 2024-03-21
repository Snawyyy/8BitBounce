#pragma once
#include <Windows.h>

struct Ball {
    float x;
    float y;
    float vx;
    float vy;
    float prevX;
    float prevY;
    float accY;
    float radius;
    float friction;
    float dampingFactor;
    float restitution;
    DWORD lastTime;
    DWORD currentTime;
    float deltaTime;
    bool isDragging;

    Ball() {
        x = GetSystemMetrics(SM_CXSCREEN) / 2;
        y = GetSystemMetrics(SM_CYSCREEN) / 2;
        vx = 20; // Initial horizontal velocity
        vy = 0; // Initial vertical velocity
        prevX = 0;
        prevY = 0;
        accY = 1.5; // Gravity (vertical acceleration)
        radius = (GetSystemMetrics(SM_CXSCREEN) / 30) / 2;
        friction = 0.99f;
        dampingFactor = 0.85f; // Damping factor for energy loss on bounce
        restitution = 0.9f; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)
        lastTime = GetTickCount();
        currentTime = 0;
        deltaTime = 0;
        isDragging = false;
    }
};


LRESULT BallWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateBallWindow(HWND hParent, HINSTANCE hInstance, int x, int y, int width, int height);

int GetTaskbarHeight();

void changeColorRandomly();
