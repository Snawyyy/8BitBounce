#pragma once

#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <objbase.h>
#include <Richedit.h>
#pragma comment(lib, "comctl32.lib")

bool QuitButton(LPARAM lParam, int buttonId);
bool MinimizeButton(LPARAM lParam);
bool DefaultButton(LPARAM lParam, const wchar_t* Text, int buttonId);
LRESULT UserButton(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool Title(HDC hdc, HWND hWnd, int centerW);
void WindowBar(HDC hdc, HWND hWnd, int width);
void WindowFrame(HDC hdc, HWND hWnd, int width, int height);

void LetterBackground(HDC hdc, HWND hWnd, int width, int height);

void RichTextBoxPaint(HWND Box);

void ServerStatusBar(HDC hdc, int isConnected, int x, int y);

 // HMENU id
#define TITLE_ID 0

#define QUIT_BUTTON_ID 1
#define MINIMIZE_BUTTON_ID 2

#define INK_LETTER_BUTTON_ID 3
#define SEND_ID 32

#define S_INITIALIZE_BUTTON_ID 4
#define S_CONNECT_BUTTON_ID 5

#define INVISBLE_TEXTBOX_SUBCLASS_ID 101

#define TIMER_UPDATE_ID 1
// learn about enum

 // Window Bar Settings
#define TITLE_SIZE 17.5
#define WIN_BAR_SIZE 30
#define BAR_MARGIN  5
#define BAR_BUTTON_SIZE  20
#define BORDER_EFFECT_SIZE 4

 // Ui Settings
#define MARGIN  20
#define SMALL_MARGIN  7.5

#define LETTER_BOX_WIDTH 650 //Letter writing box
#define LETTER_BOX_HEIGHT 400 
#define LETTER_BOX_CAP 1400 // Max letters in the letter box

#define BUTTON_WIDTH GetSystemMetrics(SM_CXSCREEN) / 15 //Button
#define BUTTON_HEIGHT GetSystemMetrics(SM_CYSCREEN) / 25

// Ui Colors

#define BACKGROUND_COLOR RGB(159, 123, 89)

// WindowBar Colors
#define WINODW_UI_COLOR RGB(48, 41, 47)
#define WINODW_UI_COLOR_SHADOW RGB(2, 17, 27)
#define WINDOW_UI_COLOR_SHINE RGB(63, 64, 69)

// Letter Colors
#define LETTER_BORDER RGB(50, 0, 0)
#define PAPER_COLOR RGB(234, 180, 100)
#define PAPER_COLOR_SHADOW RGB(167, 117, 77)

// UI Colors
#define UI_BORDER RGB(50, 0, 0)
#define UI_BORDER_SHINE RGB(150, 100, 70)
#define UI_BORDER_SHADOW RGB(100, 50, 50)

// Server Status Colors
#define STATUS_OFFLINE_COLOR RGB(255, 100, 100)
#define STATUS_ONLINE_COLOR RGB(100, 255, 100)
#define STATUS_Server_COLOR RGB(150, 150, 255)

#define DEFULT_BUTTON_COLOR RGB(240, 220, 200)
#define DEFULT_BUTTON_COLOR_SHADOW RGB(160, 120, 95)
#define DEFULT_BUTTON_COLOR_SHINE RGB(255, 240, 200) 
#define DEFULT_BUTTON_COLOR_PRESSED RGB(200, 200, 255)
#define DEFULT_BUTTON_COLOR_PRESSED_TEXT RGB(255, 255, 255)










