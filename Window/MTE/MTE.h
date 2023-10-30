#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "resource.h"

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 480

#define BUTTON_COUNT 5
#define DFBUTTON_WIDTH 160
#define DFBUTTON_HEIGHT 64

#define POLY_RECTANGLE 1
#define POLY_ELLIPSE 2
#define POLY_BONO 3
#define POLY_RYAN 4
#define POLY_CUBE 5

struct POLY {
    int left;
    int top;
    int right;
    int bottom;
    int shape;
    bool h_reverse;
    bool v_reverse;
};

void savePoly(POLY& rect, int sx, int sy, int ex, int ey, int shape) {
    rect.shape = shape;
        if (sx > ex) {
            rect.left = ex;
            rect.right = sx;
            rect.h_reverse = true;
        }
        else {
            rect.left = sx;
            rect.right = ex;
            rect.h_reverse = false;
        }
        if (sy > ey) {
            rect.top = ey;
            rect.bottom = sy;
            rect.v_reverse = true;
        }
        else {
            rect.top = sy;
            rect.bottom = ey;
            rect.v_reverse = false;
        }
}

void DrawBonoBono(HDC hdc, int x, int y, int a, int b, BOOL closeEyes) {
    //보노보노 얼굴 
    HBRUSH hbrush = CreateSolidBrush(RGB(127, 200, 255));
    SelectObject(hdc, hbrush);
    Ellipse(hdc, x - a, y - b, x + a, y + b);
    int angle;
    if (!closeEyes) {
        //보노보노 눈 그리기
        angle = -140;
        hbrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hbrush);
        for (int i = 0; i < 2; ++i) {
            double radian = (angle * i - 20) * M_PI / 180.0;
            int px = x + static_cast<int>(a * 4 / 5 * cos(radian));
            int py = y + static_cast<int>(b * 4 / 5 * sin(radian));
            Ellipse(hdc, px - a / 16, py - b / 16, px + a / 16, py + b / 16);
        }
        hbrush = CreateSolidBrush(RGB(255, 150, 255));
        SelectObject(hdc, hbrush);
        Ellipse(hdc, x - a / 5, y + b / 5, x + a / 5, b * 2 / 5 + y + b * 2 / 5);
    }
    else {
        //보노보노 감은 눈
        angle = -140;
        hbrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hbrush);
        for (int i = 0; i < 2; ++i) {
            double radian = (angle * i - 20) * M_PI / 180.0;
            int px = x + static_cast<int>(a * 4 / 5 * cos(radian));
            int py = y + static_cast<int>(b * 4 / 5 * sin(radian));
            MoveToEx(hdc, i == 0 ? px - a / 16 : px + a / 16, py, NULL);
            LineTo(hdc, i == 0 ? px + a / 16 : px - a / 16, py - b / 16);
            MoveToEx(hdc, i == 0 ? px - a / 16 : px + a / 16, py, NULL);
            LineTo(hdc, i == 0 ? px + a / 16 : px - a / 16, py + b / 16);
        }
        hbrush = CreateSolidBrush(RGB(255, 150, 255));
        SelectObject(hdc, hbrush);
        Ellipse(hdc, x - a / 5, y + b / 5, x + a / 5, b * 2 / 5 + y + b * 2 / 5);
    }

    //보노보노 주둥이 그리기
    POINT whiskers[3][2];
    angle = 60;
    hbrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hbrush);
    for (int i = 0; i < 2; ++i) {
        double radian = (angle * i + 60) * M_PI / 180.0;
        int px = x + static_cast<int>(a * 1 / 2 * cos(radian));
        int py = y + static_cast<int>(b * 1 / 2 * sin(radian));
        Ellipse(hdc, px - a / 4, py - b / 3, px + a / 4, py + b / 5);
        //보노보노 수염 그리기
        for (int j = 0; j < 3; ++j) {
            whiskers[j][0] = { px - 2 * i * a / 8 + a / 8, (py - b * 1 / 15) - b / 8 + j * b / 8 };
            whiskers[j][1] = { px - 2 * i * a / 8 + a / 8 - 2 * i * a / 4 + a / 4, (py - b * 1 / 15) - b / 8 + j * b / 8 + (j !=1 ? j * b / 8 - b / 8 : 0)};
            MoveToEx(hdc, whiskers[j][0].x, whiskers[j][0].y, NULL);
            LineTo(hdc, whiskers[j][1].x, whiskers[j][1].y);
        }
    }
    //보노보노 코 그리기
    hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    Ellipse(hdc, x - a / 5, y, x + a / 5, b * 2 / 5 + y + b / 5 - b / 4);
    DeleteObject(hbrush);
}

void DrawRyan(HDC hdc, int x, int y, int a, int b) {
    HBRUSH hbrush = CreateSolidBrush(RGB(255, 200, 15));
    HPEN hpen = CreatePen(BS_SOLID, 4, RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    SelectObject(hdc, hpen);
    //라이언 귀 그리기
    double angle = -80;
    for (int i = 0; i < 2; ++i) {
        double radian = (angle * i - 50) * M_PI / 180.0;
        int px = x + static_cast<int>(a * cos(radian));
        int py = y + static_cast<int>(b * sin(radian));
        Ellipse(hdc, px - a / 4, py - b / 4, px + a / 4, py + b / 4);
    }
    //라이언 얼굴 그리기
    Ellipse(hdc, x - a, y - b, x + a, y + b);

    //라이언 눈,눈썹 그리기
    DeleteObject(hpen);
    hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    angle = -140;
    for (int i = 0; i < 2; ++i) {
        double radian = (angle * i - 20) * M_PI / 180.0;
        int px = x + static_cast<int>(a / 2 * cos(radian));
        int py = y + static_cast<int>(b / 2 * sin(radian));
        Ellipse(hdc, px - a / 12, py - b / 12, px + a / 12, py + b / 12);
        Rectangle(hdc, px - a / 5, py - b * 1 / 3, px + a / 5, py - b * 1 / 3 + b / 12);
        int rds = b / 24;
        Ellipse(hdc, px - a / 5 - rds, py - b * 1 / 3, px - a / 5 + rds, py - b * 1 / 3 + b / 12);
        Ellipse(hdc, px + a / 5 - rds, py - b * 1 / 3, px + a / 5 + rds, py - b * 1 / 3 + b / 12);
    }


    //라이언 주둥이 그리기
    hpen = CreatePen(BS_SOLID, 5, RGB(0, 0, 0));
    SelectObject(hdc, hpen);
    angle = 40;
    hbrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hbrush);
    POINT points[2];
    for (int i = 0; i < 2; ++i) {
        double radian = (angle * i + 70) * M_PI / 180.0;
        int px = x + static_cast<int>(a * 1 / 2 * cos(radian));
        int py = y + static_cast<int>(b * 1 / 2 * sin(radian));
        Ellipse(hdc, px - a / 5, y + b / 8, px + a / 5, y + b / 8 + b * 2 / 5);
        points[i] = { px, py };
    }
    RECT rt = { points[0].x, y + b / 4, points[1].x, y + b / 4 + b / 6 };
    FillRect(hdc, &rt, hbrush);
    DeleteObject(hpen);
    //라이언 코 그리기
    hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    Ellipse(hdc, x - a / 8, y + b / 16, x + a / 8, y + b / 4);
    DeleteObject(hbrush);
}


void DrawCube(HDC hdc, int sx, int sy, int ex, int ey) {
    POINT points[12];
    double ratio = 0.7;
    int width = (ex - sx) * ratio;
    int height = (ey - sy) * ratio;
    points[0] = { sx, ey - height };
    points[1] = { sx, ey };
    points[2] = { sx + width, ey };
    points[3] = { points[2].x, points[0].y };
    points[4] = points[0];

    points[5] = { ex - width, sy };
    points[6] = { ex, sy };
    points[7] = { ex, sy + height };
    points[8] = points[2];
    points[9] = points[3];
    points[10] = points[6];
    points[11] = points[3];

    Polygon(hdc, points, 12);

}