#include "CustomWindowUI.h"

bool QuitButton(LPARAM lParam, int buttonId) // Bar Quit Button
{
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;

	if (pDIS->CtlID == buttonId) // Matching the HMENU value passed when creating the button
	{
		BOOL isPressed = pDIS->itemState & ODS_SELECTED;

		// Set the background and text colors
		SetTextColor(pDIS->hDC, RGB(0, 0, 0));
		SetBkMode(pDIS->hDC, TRANSPARENT);
		FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(255, 0, 0)));

		// Button shading
		RECT effectRect;

		// Shadow
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.bottom - BORDER_EFFECT_SIZE, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(100, 0, 0)));
		SetRect(&effectRect, pDIS->rcItem.right - BORDER_EFFECT_SIZE, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(100, 0, 0))); 

		// Shine
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.top + BORDER_EFFECT_SIZE); 
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(255, 100, 100)));
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.left + BORDER_EFFECT_SIZE, pDIS->rcItem.bottom); 
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(255, 100, 100)));

		// Prepare the rectangle for the text, adjusting if the button is pressed
		RECT textRect = pDIS->rcItem;
		if (isPressed) {
			// Offset the textRect and change color to simulate the text moving when pressed
			OffsetRect(&textRect, 1, 1);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SetBkColor(pDIS->hDC, RGB(100, 0, 0));
			FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(100, 0, 0)));
		}

		// Draw the button text using the adjusted rectangle
		DrawText(pDIS->hDC, L"X", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FrameRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(100, 0, 0))); // Draw the border around the button


		return TRUE; // Indicate we handled the message
	}
	else
	{
		return FALSE;
	}
}

bool MinimizeButton(LPARAM lParam) // Bar Minimize Button
{
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
	if (pDIS->CtlID == MINIMIZE_BUTTON_ID) // Matching the HMENU value passed when creating the button
	{ 

		BOOL isPressed = pDIS->itemState & ODS_SELECTED;

		// Set the background and text colors
		SetTextColor(pDIS->hDC, RGB(0, 0, 0));
		SetBkMode(pDIS->hDC, TRANSPARENT);
		FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(255, 250, 215)));

		// Button shading
		RECT effectRect;

		// Shadow
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.bottom - BORDER_EFFECT_SIZE, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(160, 120, 95)));
		SetRect(&effectRect, pDIS->rcItem.right - BORDER_EFFECT_SIZE, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(160, 120, 95)));

		// Shine
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.top + BORDER_EFFECT_SIZE);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(255, 240, 200)));
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.left + BORDER_EFFECT_SIZE, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(RGB(255, 240, 200)));

		// Prepare the rectangle for the text, adjusting if the button is pressed
		RECT textRect = pDIS->rcItem;
		if (isPressed) {
			// Offset the textRect and change color to simulate the text moving when pressed
			OffsetRect(&textRect, 1, 1);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SetBkColor(pDIS->hDC, RGB(200, 200, 255));
			FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(200, 200, 255)));
		}

		// Draw the button text using the adjusted rectangle
		DrawText(pDIS->hDC, L"-", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		FrameRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(100, 0, 0))); // Draw the border around the button


		return TRUE; // Indicate we handled the message
	}
}

bool DefaultButton(LPARAM lParam, const wchar_t* Text, int buttonId) // GUI Default Button
{
	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
	if (pDIS->CtlID == buttonId)// Matching the HMENU value passed when creating the button
	{

		BOOL isPressed = pDIS->itemState & ODS_SELECTED;

		// Set the background and text colors
		SetTextColor(pDIS->hDC, RGB(0, 0, 0));
		SetBkMode(pDIS->hDC, TRANSPARENT);
		FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(DEFULT_BUTTON_COLOR));

		// Button shading
		RECT effectRect;

		// Shadow
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.bottom - BORDER_EFFECT_SIZE, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(DEFULT_BUTTON_COLOR_SHADOW));
		SetRect(&effectRect, pDIS->rcItem.right - BORDER_EFFECT_SIZE, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(DEFULT_BUTTON_COLOR_SHADOW));

		// Shine
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.top + BORDER_EFFECT_SIZE);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(DEFULT_BUTTON_COLOR_SHINE));
		SetRect(&effectRect, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.left + BORDER_EFFECT_SIZE, pDIS->rcItem.bottom);
		FillRect(pDIS->hDC, &effectRect, CreateSolidBrush(DEFULT_BUTTON_COLOR_SHINE));

		// Prepare the rectangle for the text, adjusting if the button is pressed
		RECT textRect = pDIS->rcItem;
		if (isPressed) {
			// Offset the textRect and change color to simulate the text moving when pressed
			OffsetRect(&textRect, 1, 1);
			SetTextColor(pDIS->hDC, DEFULT_BUTTON_COLOR_PRESSED_TEXT);
			SetBkColor(pDIS->hDC, DEFULT_BUTTON_COLOR_PRESSED);
			FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(DEFULT_BUTTON_COLOR_PRESSED));
		}

		// Draw the button text using the adjusted rectangle
		DrawText(pDIS->hDC, Text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// Draw a border around the button
		FrameRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(0, 0, 0))); // Draw the border around the button

		return TRUE; // Indicate we handled the message
	}
}


LRESULT CALLBACK UserButton(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush = NULL;
	static RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	static bool isPressed;

	switch (msg)
	{
	case WM_CREATE:
	{
		// Creates a solid brush for the button background color
		hBrush = CreateSolidBrush(UI_BORDER); // Change the RGB values to set your desired background color
		break;
	}
	case WM_SIZE:
	{
		// Stores the button's client area dimensions
		GetClientRect(hwnd, &rect);

		// Create a circular region based on the updated client rect
		HRGN hRgn = CreateEllipticRgn(0, 0, rect.right, rect.bottom);
		SetWindowRgn(hwnd, hRgn, TRUE); // Set the window region. TRUE to redraw the window immediately

		// NOTE: The region is now owned by the system, no need to call DeleteObject on hRgn
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		// Create the outer and inner rectangles
		RECT outerRect = { 0, 0, rect.right, rect.bottom };
		RECT innerRect = { BORDER_EFFECT_SIZE, BORDER_EFFECT_SIZE, rect.right - BORDER_EFFECT_SIZE, rect.bottom - BORDER_EFFECT_SIZE };

		// Create the outer and inner regions
		HRGN outerRgn = CreateEllipticRgnIndirect(&outerRect);
		HRGN innerRgn = CreateEllipticRgnIndirect(&innerRect);

		// Combine the regions to create a ring shape
		CombineRgn(outerRgn, outerRgn, innerRgn, RGN_DIFF);
		if (isPressed)
		{
			// Set the brush color for the outer ring
			HBRUSH outerBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, outerBrush);

			// Fill the outer ring
			FillRgn(hdc, outerRgn, outerBrush);

			// Set the brush color for the inner circle
			HBRUSH innerBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, innerBrush);

			// Fill the inner circle
			Ellipse(hdc, innerRect.left, innerRect.top, innerRect.right, innerRect.bottom);
			FillRgn(hdc, innerRgn, innerBrush);
			// Clean up the brushes and regions
			DeleteObject(outerBrush);
			DeleteObject(innerBrush);
		}
		else
		{
			// Set the brush color for the outer ring
			HBRUSH outerBrush = CreateSolidBrush(UI_BORDER);
			SelectObject(hdc, outerBrush);

			// Fill the outer ring
			FillRgn(hdc, outerRgn, outerBrush);

			// Set the brush color for the inner circle
			HBRUSH innerBrush = CreateSolidBrush(UI_BORDER_SHADOW);
			SelectObject(hdc, innerBrush);

			// Fill the inner circle
			Ellipse(hdc, innerRect.left, innerRect.top, innerRect.right, innerRect.bottom);
			FillRgn(hdc, innerRgn, innerBrush);
			// Clean up the brushes and regions
			DeleteObject(outerBrush);
			DeleteObject(innerBrush);
		}
		DeleteObject(outerRgn);
		DeleteObject(innerRgn);

		// Draw the button text centered in the circle
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, L"Button", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		break;


	}
	case WM_MOUSEMOVE:
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE; // Specifies that we want a WM_MOUSELEAVE message when the mouse leaves
		tme.hwndTrack = hwnd;
		tme.dwHoverTime = HOVER_DEFAULT; // Not needed for WM_MOUSELEAVE but required to be set

		TrackMouseEvent(&tme); // Call this function to start tracking

		break;
	}
	case WM_MOUSELEAVE:
	{
		// The mouse has left the window, you can handle it here
		isPressed = FALSE; // Reset the isPressed state if needed
		InvalidateRect(hwnd, NULL, TRUE); // Redraw the window if necessary

		break;
	}
	case WM_LBUTTONDOWN:
	{
		// Update the button's pressed state
		isPressed = TRUE;
		// Invalidate the button to trigger a redraw
		InvalidateRect(hwnd, NULL, TRUE);

		break;
	}
	case WM_LBUTTONUP:
	{
		// Update the button's pressed state
		isPressed = FALSE;
		SendMessage(GetParent(hwnd), WM_COMMAND, (WPARAM)7, (LPARAM)hwnd);
		// Invalidate the button to trigger a redraw
		InvalidateRect(hwnd, NULL, TRUE);

		break;
	}
	case WM_DESTROY:
	{
		// Clean up resources
		DeleteObject(hBrush);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

bool Title(HDC hdc, HWND hWnd, int centerW) // The title of the application in the GUI
{
	// Custom drawing code goes here
	HFONT hFont = CreateFont(
		TITLE_SIZE,               // Height of the font
		0,                     // Average character width (0 lets the system choose the best value)
		0,                     // Angle of escapement
		0,                     // Base-line orientation angle
		FW_BOLD,               // Font weight (FW_BOLD for bold)
		FALSE,                 // Italic attribute option
		FALSE,                 // Underline attribute option
		FALSE,                 // Strikeout attribute option
		ANSI_CHARSET,          // Character set identifier
		OUT_DEFAULT_PRECIS,    // Output precision
		CLIP_DEFAULT_PRECIS,   // Clipping precision
		NONANTIALIASED_QUALITY,       // Output quality
		DEFAULT_PITCH | FF_SWISS, // Pitch and family
		L"Arial");              // Font name
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	SetTextColor(hdc, PAPER_COLOR);// text color
	SetBkMode(hdc, TRANSPARENT); // To make background transparent
	TextOut(hdc, SMALL_MARGIN + BORDER_EFFECT_SIZE * 2, ((WIN_BAR_SIZE / 2) - (TITLE_SIZE / 2) + 1), L"Pixel Letter Station", strlen("Pixel Letter Station"));
	return 0; // Indicate we handled the message


}

void WindowBar(HDC hdc, HWND hWnd, int width)
{
	// Set the color for the rectangle (optional)
	HBRUSH brush = CreateSolidBrush(WINODW_UI_COLOR); // orangeish color
	HPEN nullPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0)); // Null pen, color doesn't matter
	SelectObject(hdc, brush);
	SelectObject(hdc, nullPen);

	// Draw the rectangle
	// Parameters: HDC, left, top, right, bottom
	Rectangle(hdc, 0, 0, width, WIN_BAR_SIZE);

	// Draw the Border Shadow
	brush = CreateSolidBrush(WINODW_UI_COLOR_SHADOW); // orangeish color
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, WIN_BAR_SIZE, width, WIN_BAR_SIZE - BORDER_EFFECT_SIZE);
	Rectangle(hdc, width - BORDER_EFFECT_SIZE + 1, 0, width + 1, WIN_BAR_SIZE);

	// Draw the Border shine
	brush = CreateSolidBrush(WINDOW_UI_COLOR_SHINE); // orangeish color
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, 0, width + 1, BORDER_EFFECT_SIZE);
	Rectangle(hdc, 0, 0, BORDER_EFFECT_SIZE, WIN_BAR_SIZE);



	// Clean up
	DeleteObject(brush);
	DeleteObject(nullPen);
}

void WindowFrame(HDC hdc, HWND hWnd, int width, int height)
{
	// Create a pen of desired thickness and color
	HPEN hPen = CreatePen(PS_SOLID, BAR_MARGIN * 2, WINODW_UI_COLOR); // Black pen

	// Select the pen and a null brush into the DC
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	// Draw the rectangle
	Rectangle(hdc, 0, 0, width, height);

	// Restore the original pen and brush
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	// Clean up
	DeleteObject(hPen);
}

void LetterBackground(HDC hdc, HWND hWnd, int width, int height)
{
	// First, draw the larger rectangle with a solid color
	HBRUSH brushMain = CreateSolidBrush(LETTER_BORDER); // Black color for the Border
	RECT rectMain = { width - LETTER_BOX_WIDTH - MARGIN - SMALL_MARGIN, MARGIN * 2.5, width - (SMALL_MARGIN * 2), height - (MARGIN * 3)}; // Main rectangle coordinates
	FillRect(hdc, &rectMain, brushMain);

	//Then, the "Paper" by drawing it with the paper color
	HBRUSH brushShading = CreateSolidBrush(PAPER_COLOR_SHADOW); // Brush for the Shadow, using the shadow color
	RECT rectShading = { (width - (LETTER_BOX_WIDTH / 2) - MARGIN + BORDER_EFFECT_SIZE) - (LETTER_BOX_WIDTH / 2) - SMALL_MARGIN, (MARGIN * 2.5) + BORDER_EFFECT_SIZE, width - (SMALL_MARGIN * 2) - BORDER_EFFECT_SIZE, height - (MARGIN * 3) - BORDER_EFFECT_SIZE }; // Smaller rectangle coordinates for the cutout
	FillRect(hdc, &rectShading, brushShading);

	//Then, the "Paper" by drawing it with the paper color
	HBRUSH brushPaper = CreateSolidBrush(PAPER_COLOR); // Brush for the cutout, using the paper background color
	RECT rectCutout = { (width - (LETTER_BOX_WIDTH / 2) - MARGIN + (BORDER_EFFECT_SIZE * 2)) - (LETTER_BOX_WIDTH / 2) - SMALL_MARGIN, (MARGIN * 2.5) + (BORDER_EFFECT_SIZE * 2), width - (SMALL_MARGIN * 2) - BORDER_EFFECT_SIZE, height - (MARGIN * 3) - BORDER_EFFECT_SIZE }; // Smaller rectangle coordinates for the cutout
	FillRect(hdc, &rectCutout, brushPaper);

	// Clean up
	DeleteObject(brushMain);
	DeleteObject(brushShading);
	DeleteObject(brushPaper);
}

void RichTextBoxPaint(HWND box)
{
	PARAFORMAT2 pf;
	memset(&pf, 0, sizeof(PARAFORMAT2)); // formatting the richbox text
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_LINESPACING;
	pf.bLineSpacingRule = 5;
	pf.dyLineSpacing = 30;

	SendMessage(box, EM_SETBKGNDCOLOR, 0, (LPARAM)PAPER_COLOR); // BKG color
	SendMessage(box, EM_SETPARAFORMAT, 0, (LPARAM)&pf); // Sends format
	SendMessage(box, EM_SETLIMITTEXT, (WPARAM)1400, 0); // Sends Text char limit

}

void ServerStatusBar(HDC hdc, int isConnected, int x, int y)
{
	int height = (MARGIN * 1.5) + SMALL_MARGIN;
	int width = MARGIN * 8.5;

	// First, draw the larger rectangle with a solid color
	HBRUSH brushMain = CreateSolidBrush(UI_BORDER); // Black color for the Border
	RECT rectMain = { x, y, x + width, y + height }; // Main rectangle coordinates
	FillRect(hdc, &rectMain, brushMain);

	//Then, the "Shine" by drawing it with the Shine color
	HBRUSH brushShading = CreateSolidBrush(UI_BORDER_SHINE); // Brush for the cutout, using the Shine color
	RECT rectShading = { x + BORDER_EFFECT_SIZE, y + BORDER_EFFECT_SIZE, x + width - BORDER_EFFECT_SIZE, y + height - BORDER_EFFECT_SIZE }; // Smaller rectangle coordinates for the cutout
	FillRect(hdc, &rectShading, brushShading);

	//Then, the "Paper" by drawing it with the paper color
	HBRUSH brushPaper = CreateSolidBrush(UI_BORDER_SHADOW); // Brush for the cutout, using the window background color
	RECT rectCutout = { x + (BORDER_EFFECT_SIZE * 2), y + (BORDER_EFFECT_SIZE * 2), x + width - BORDER_EFFECT_SIZE, y + height - BORDER_EFFECT_SIZE }; // Smaller rectangle coordinates for the cutout
	FillRect(hdc, &rectCutout, brushPaper);


	// Custom drawing code goes here
	HFONT hFont = CreateFont(
		TITLE_SIZE,               // Height of the font
		0,                     // Average character width (0 lets the system choose the best value)
		0,                     // Angle of escapement
		0,                     // Base-line orientation angle
		FW_BOLD,               // Font weight (FW_BOLD for bold)
		FALSE,                 // Italic attribute option
		FALSE,                 // Underline attribute option
		FALSE,                 // Strikeout attribute option
		ANSI_CHARSET,          // Character set identifier
		OUT_DEFAULT_PRECIS,    // Output precision
		CLIP_DEFAULT_PRECIS,   // Clipping precision
		NONANTIALIASED_QUALITY,       // Output quality
		DEFAULT_PITCH | FF_SWISS, // Pitch and family
		L"Arial");              // Font name
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	SetTextColor(hdc, RGB(255, 255, 255));// text color
	SetBkMode(hdc, TRANSPARENT); // To make background transparent
	TextOut(hdc, x + SMALL_MARGIN + BORDER_EFFECT_SIZE, y + (height / 2) + BORDER_EFFECT_SIZE - (TITLE_SIZE / 2), L"Status:", strlen("Status:"));
	if (isConnected == 0)
	{
		SetTextColor(hdc, STATUS_OFFLINE_COLOR);// text color
		TextOut(hdc, x + MARGIN * 3, y + (height / 2) + BORDER_EFFECT_SIZE - (TITLE_SIZE / 2), L"Offline", strlen("Offline"));
	}
	if (isConnected == 1)
	{
		SetTextColor(hdc, STATUS_ONLINE_COLOR);// text color
		TextOut(hdc, x + MARGIN * 3, y + (height / 2) + BORDER_EFFECT_SIZE - (TITLE_SIZE / 2), L"Connected", strlen("Connected"));
	}
	if (isConnected == 2)
	{
		SetTextColor(hdc, STATUS_Server_COLOR);// text color
		TextOut(hdc, x + MARGIN * 3, y + (height / 2) + BORDER_EFFECT_SIZE - (TITLE_SIZE / 2), L"server", strlen("server"));
	}

	// Clean up
	DeleteObject(brushMain);
	DeleteObject(brushShading);
	DeleteObject(brushPaper);

	return;

}