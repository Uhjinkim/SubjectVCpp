#pragma once

#include "resource.h"
#include "BonoBono.h"
void DrawBonobono(HWND hWnd, HDC hdc, int blink);

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom);

void DrawBonobono(HWND hWnd, HDC hdc, int blink) {
	RECT rect;
	GetClientRect(hWnd, &rect);
	int centerX = (rect.left + rect.right) / 2;
	int centerY = (rect.top + rect.bottom) / 2;
	DrawBonoBono(hdc, centerX, centerY, 100, 100, blink);
}

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
	int centerX = (left + right) / 2;
	int centerY = (top + bottom) / 2;
	int hRadius = (right - left)/2;
	int vRadius = (bottom - top)/2;
	if (bottom < top) {
		DrawRyan(hdc, centerX, centerY, bottom, top, hRadius, vRadius);
	}
	else {
		DrawRyan(hdc, centerX, centerY, top, bottom, hRadius, vRadius);
	}
}
