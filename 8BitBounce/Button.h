#pragma once
#include "Ui.h"


class Button : public Ui
{
public:
	LPARAM lParam;
	LPDRAWITEMSTRUCT pDIS;
	int buttonId;
	const wchar_t* text;

	Button(LPARAM lp, int id, const wchar_t* te) :Ui(hWnd), lParam(lp), buttonId(id), text(te)
	{
		pDIS = (LPDRAWITEMSTRUCT)lParam;
	}

	virtual void Draw(COLORREF backgroundColor, COLORREF textColor)
	{
		if (pDIS->CtlID == buttonId)// Matching the HMENU value passed when creating the button
		{
			DrawBackground(backgroundColor, textColor);
			DrawShading(backgroundColor);
			DrawButtonText(textColor);
			DrawPressed(backgroundColor);
			DrawBorder();
		}
	}

	virtual void DrawBorder()
	{
		FrameRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(1, 0, 0))); // Draw the border around the button
	}

	void DrawBackground(COLORREF backgroundColor, COLORREF textColor)
	{
		SetBkMode(pDIS->hDC, TRANSPARENT);
		FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(backgroundColor));
	}

	void DrawShading(COLORREF color)
	{
		float factor = 0.5f;
		COLORREF shadow;
		COLORREF shine;

		// Button shading
		RECT effectRect;

		shadow = GetShadow(color, factor);
		shine = GetShine(color, factor);

		// Shadow
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.bottom - BORDER_EFFECT_SIZE, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(shadow));
		SetRect(&effectRect, pDIS->rcItem.right - BORDER_EFFECT_SIZE, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(shadow));

		// Shine
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.top + BORDER_EFFECT_SIZE);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(shine));
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.left + BORDER_EFFECT_SIZE, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(shine));
	}

	void DrawButtonText(COLORREF textColor)
	{
		RECT textRect = pDIS->rcItem;
		SetTextColor(pDIS->hDC, textColor);
		DrawText(pDIS->hDC, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	void DrawPressed(COLORREF color)
	{
		// Prepare the rectangle for the text, adjusting if the button is pressed
		RECT textRect = pDIS->rcItem;
		BOOL isPressed = pDIS->itemState & ODS_SELECTED;

		if (isPressed) {
			// Offset the textRect and change color to simulate the text moving when pressed
			OffsetRect(&textRect, 1, 1);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(GetShadow(color, 0.5)));
			DrawText(pDIS->hDC, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
};








