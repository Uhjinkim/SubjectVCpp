#pragma once

#include "resource.h"
#include "widgets.h"

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 480
#define BUTTON_COUNT 5
#define DFBUTTON_WIDTH 160
#define DFBUTTON_HEIGHT 64

//도형 상태 상수
#define POLY_RECTANGLE 1
#define POLY_ELLIPSE 2
#define POLY_BONO 3
#define POLY_RYAN 4
#define POLY_CUBE 5

struct ButtonInfo {
    WCHAR name[BUTTON_COUNT][10] = { L"직사각형", L"타원", L"보노보노", L"라이언", L"큐브" };
    int width = DFBUTTON_WIDTH;
    int height = DFBUTTON_HEIGHT;
    int margin = 16; //버튼의 마진
    int menu[BUTTON_COUNT] = { POLY_RECTANGLE, POLY_ELLIPSE, POLY_BONO, POLY_RYAN, POLY_CUBE };
};