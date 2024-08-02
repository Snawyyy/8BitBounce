#pragma once
#include "Ui.h"

class Button : public Ui
{
public:
    LPARAM lParam;
    LPDRAWITEMSTRUCT pDIS;
    int buttonId;
    const wchar_t* text;

    Button(LPARAM lp, int id, const wchar_t* te) : Ui(nullptr), lParam(lp), buttonId(id), text(te)
    {
        pDIS = (LPDRAWITEMSTRUCT)lParam;
    }

    virtual void Draw(COLORREF backgroundColor, COLORREF textColor)
    {
        if (pDIS && pDIS->CtlID == buttonId)
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
        if (pDIS)
        {
            HBRUSH hBrush = CreateSolidBrush(RGB(1, 0, 0));
            FrameRect(pDIS->hDC, &pDIS->rcItem, hBrush);
            DeleteObject(hBrush);
        }
    }

    void DrawBackground(COLORREF backgroundColor, COLORREF textColor)
    {
        if (pDIS)
        {
            SetBkMode(pDIS->hDC, TRANSPARENT);
            HBRUSH hBrush = CreateSolidBrush(backgroundColor);
            FillRect(pDIS->hDC, &pDIS->rcItem, hBrush);
            DeleteObject(hBrush);
        }
    }

    void DrawShading(COLORREF color)
    {
        if (pDIS)
        {
            float factor = 0.5f;
            COLORREF shadow = GetShadow(color, factor);
            COLORREF shine = GetShine(color, factor);

            RECT effectRect;
            HBRUSH hShadowBrush = CreateSolidBrush(shadow);
            HBRUSH hShineBrush = CreateSolidBrush(shine);

            // Shadow
            SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.bottom - BORDER_EFFECT_SIZE, pDIS->rcItem.right, pDIS->rcItem.bottom);
            FillRect(pDIS->hDC, &effectRect, hShadowBrush);
            SetRect(&effectRect, pDIS->rcItem.right - BORDER_EFFECT_SIZE, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
            FillRect(pDIS->hDC, &effectRect, hShadowBrush);

            // Shine
            SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.top + BORDER_EFFECT_SIZE);
            FillRect(pDIS->hDC, &effectRect, hShineBrush);
            SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.left + BORDER_EFFECT_SIZE, pDIS->rcItem.bottom);
            FillRect(pDIS->hDC, &effectRect, hShineBrush);

            DeleteObject(hShadowBrush);
            DeleteObject(hShineBrush);
        }
    }

    void DrawButtonText(COLORREF textColor)
    {
        if (pDIS)
        {
            RECT textRect = pDIS->rcItem;
            SetTextColor(pDIS->hDC, textColor);
            DrawText(pDIS->hDC, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    void DrawPressed(COLORREF color)
    {
        if (pDIS)
        {
            RECT textRect = pDIS->rcItem;
            BOOL isPressed = pDIS->itemState & ODS_SELECTED;

            if (isPressed) {
                OffsetRect(&textRect, 1, 1);
                SetTextColor(pDIS->hDC, RGB(255, 255, 255));
                HBRUSH hBrush = CreateSolidBrush(GetShadow(color, 0.5));
                FillRect(pDIS->hDC, &pDIS->rcItem, hBrush);
                DeleteObject(hBrush);
                DrawText(pDIS->hDC, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
    }
};