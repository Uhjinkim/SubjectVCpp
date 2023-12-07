#pragma once

#include "resource.h"
#include "widgets.h"
#include "Bonobono.h"

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 480
#define BUTTON_COUNT 5


void DrawBonobono(HWND hWnd, HDC hdc, int blink);

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom);

void DrawBonobono(HWND hWnd, HDC hdc, int blink) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	int centerX = (rect.left + rect.right) / 2;
	int centerY = (rect.top + rect.bottom) / 2;
	DrawBonobono(hdc, centerX, centerY, 100, 100, blink);
}

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
	int centerX = (left + right) / 2;
	int centerY = (top + bottom) / 2;
	int hRadius = (right - left) / 2;
	int vRadius = (bottom - top) / 2;
	if (bottom < top) {
		DrawRyan(hdc, centerX, centerY, bottom, top, hRadius, vRadius);
	}
	else {
		DrawRyan(hdc, centerX, centerY, top, bottom, hRadius, vRadius);
	}
}

int getRadiusHorizontal(int x1, int x2) {
	return x1 < x2 ? (x2 - x1) / 2 : (x1 - x2) / 2;
}
int getRadiusVertical(int y1, int y2) {
	return y1 < y2 ? (y2 - y1) / 2 : (y1 - y2) / 2;
}
int getCenterX(int x1, int x2) {
	return (x1 + x2) / 2;
}
int getCenterY(int y1, int y2) {
	return (y1 + y2) / 2;
}
void DebugPrint(const wchar_t* message)
{
	OutputDebugString(message);
	OutputDebugString(L"\n");
}