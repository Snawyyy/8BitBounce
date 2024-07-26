#pragma once
#include <Windows.h>
#include <corecrt_math.h>

POINT GetPos(HWND hWnd);

static POINT GetDifference(POINT p1, POINT p2);

POINT GetSymmetricalPoint(POINT origin, POINT target);

bool IsFarEnough(POINT pos, float distance);

void Step(HWND hWnd);

