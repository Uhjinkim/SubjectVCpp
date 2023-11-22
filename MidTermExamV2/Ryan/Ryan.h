#pragma once

#include "resource.h"
#include "widgets.h"

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 480
#define BUTTON_COUNT 5
#define DFBUTTON_WIDTH 160
#define DFBUTTON_HEIGHT 64

//���� ���� ���
#define POLY_RECTANGLE 1
#define POLY_ELLIPSE 2
#define POLY_BONO 3
#define POLY_RYAN 4
#define POLY_CUBE 5

struct ButtonInfo {
    WCHAR name[BUTTON_COUNT][10] = { L"���簢��", L"Ÿ��", L"���뺸��", L"���̾�", L"ť��" };
    int width = DFBUTTON_WIDTH;
    int height = DFBUTTON_HEIGHT;
    int margin = 16; //��ư�� ����
    int menu[BUTTON_COUNT] = { POLY_RECTANGLE, POLY_ELLIPSE, POLY_BONO, POLY_RYAN, POLY_CUBE };
};