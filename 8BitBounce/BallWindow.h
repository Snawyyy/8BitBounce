#pragma once
#include <Windows.h>

#define BALL_SIZE GetSystemMetrics(SM_CXSCREEN) / 30

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
        POINT pt;
        GetCursorPos(&pt);

        vx = 0; // Initial horizontal velocity
        vy = 0; // Initial vertical velocity
        prevX = 0;
        prevY = 0;
        accY = 1.5; // Gravity (vertical acceleration)
        radius = BALL_SIZE / 2;
        friction = 0.99f;
        dampingFactor = 0.85f; // Damping factor for energy loss on bounce
        restitution = 0.9f; // Restitution coefficient (0 to 1, where 1 is perfectly elastic)
        lastTime = GetTickCount();
        currentTime = 0;
        deltaTime = 0;
        x = pt.x - radius;
        y = pt.y - radius;
        isDragging = false;
    }
};


LRESULT BallWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateBallWindow(HWND hParent, HINSTANCE hInstance, int x, int y, int width, int height);

int GetTaskbarHeight();

void changeColorRandomly();
