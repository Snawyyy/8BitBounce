#pragma once
#include <Windows.h>
#include "UiSettings.h"

class Ui
{
public:
	HWND hWnd;
	float factor = 0.5;

	Ui(HWND hw) :hWnd(hw)
	{

	}

	void Draw()
	{

	}

	int GetWidth(HWND hWnd)
	{
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		int width = rcClient.right - rcClient.left;
		return width;
	}

	int GetHeight(HWND hWnd)
	{
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		int height = rcClient.bottom - rcClient.top;
		return height;
	}

	COLORREF GetShadow(COLORREF color, float factor)
	{
		if (factor > 1.0)
		{
			factor = 1.0;
		}
		if (factor < 0.0)
		{
			factor = 0.0;
		}
		BYTE red = GetRValue(color);
		BYTE green = GetGValue(color);
		BYTE blue = GetBValue(color);
		int hueShift = 35;

		red = static_cast<BYTE>(red * factor);
		green = static_cast<BYTE>(green * factor);
		blue = static_cast<BYTE>(blue * factor);

		return RGB(red + hueShift, green, blue);
	}

	COLORREF GetShine(COLORREF color, float factor)
	{
		if (factor > 1.0)
		{
			factor = 1.0;
		}
		if (factor < 0.0)
		{
			factor = 0.0;
		}
		BYTE red = GetRValue(color);
		BYTE green = GetGValue(color);
		BYTE blue = GetBValue(color);
		int hueShift = 35;

		red = static_cast<BYTE>(red + (255 - red) * factor);
		green = static_cast<BYTE>(green + (255 - green) * factor);
		blue = static_cast<BYTE>(blue + (255 - blue) * factor);

		return RGB(red, green, blue - hueShift);
	}

	void DrawFrame(HDC hdc)
	{
		int width = GetWidth(hWnd);
		int height = GetHeight(hWnd);

		// Create a pen of desired thickness and color
		HPEN hPen = CreatePen(PS_SOLID, BAR_MARGIN * 2, WINODW_UI_COLOR);

		// Select the pen and a null brush into the DC
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

		// Draw the rectangle
		Rectangle(hdc, 0, 0, width, height);

		// Draw smaller Darker rectangle for shading
		hPen = CreatePen(PS_SOLID, BORDER_EFFECT_SIZE, GetShadow(WINODW_UI_COLOR, factor));
		hOldPen = (HPEN)SelectObject(hdc, hPen);

		Rectangle(hdc, 0, 0, width, height);

		hPen = CreatePen(PS_SOLID, BORDER_EFFECT_SIZE, GetShine(WINODW_UI_COLOR, factor));
		hOldPen = (HPEN)SelectObject(hdc, hPen);

		Rectangle(hdc, 0, 0, 1, height);

		// Restore the original pen and brush
		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);

		// Clean up
		DeleteObject(hPen);
	}
};