#include "win_procedural_anim.h"

POINT GetPos(HWND hWnd)
{
    RECT rect;
    GetWindowRect(hWnd, &rect);

    POINT pt;
    pt.x = rect.left;
    pt.y = rect.top;

    return pt;
}

static POINT GetDifference(POINT p1, POINT p2)
{
    POINT diff;
    diff.x = p1.x - p2.x;
    diff.y = p1.y - p2.y;
    return diff;
}

bool IsFarEnough(POINT origin, POINT target, float distance)
{
    POINT diff = GetDifference(origin, target);
    float squaredDistance = static_cast<float>(diff.x * diff.x + diff.y * diff.y);
    return squaredDistance >= (distance * distance);
}

POINT GetSymmetricalPoint(POINT origin, POINT target)
{
    POINT diff = GetDifference(target, origin);
    POINT result;
    result.x = origin.x - diff.x;
    result.y = origin.y - diff.y;
    return result;
}

void Step(HWND hWnd, POINT shoulder, float stepSize)
{
    POINT currentPos = GetPos(hWnd);

    if (!IsFarEnough(currentPos, shoulder, stepSize))
    {
        SetWindowPos(hWnd, NULL, shoulder.x, shoulder.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        return;
    }

    POINT direction = GetDifference(shoulder, currentPos);

    float length = sqrt(static_cast<float>(direction.x * direction.x + direction.y * direction.y));
    float normalizedX = direction.x / length;
    float normalizedY = direction.y / length;

    POINT newPos;
    newPos.x = currentPos.x + static_cast<LONG>(normalizedX * stepSize);
    newPos.y = currentPos.y + static_cast<LONG>(normalizedY * stepSize);

    SetWindowPos(hWnd, NULL, newPos.x, newPos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
