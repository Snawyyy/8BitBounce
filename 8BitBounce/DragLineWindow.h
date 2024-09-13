#ifndef DRAG_LINE_WINDOW_H
#define DRAG_LINE_WINDOW_H

#include <windows.h>

class DragLineWindow
{
public:
    DragLineWindow(HINSTANCE hInstance);
    ~DragLineWindow();

    HWND GetHWND() const { return hWnd; }
    void Show();
    void Hide();
    void UpdatePosition(POINT startPoint, POINT endPoint);

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND hWnd;
    HINSTANCE hInstance;

    POINT startPoint;
    POINT endPoint;
};

#endif // DRAG_LINE_WINDOW_H
