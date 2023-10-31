#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "resource.h"

//클라이언트 영역 지정을 위한 너비, 높이 상수
#define VIEW_WIDTH 800
#define VIEW_HEIGHT 480

//총 버튼 개수, 버튼 너비, 높이
#define BUTTON_COUNT 5
#define DFBUTTON_WIDTH 160
#define DFBUTTON_HEIGHT 64

//도형 상태 상수
#define POLY_RECTANGLE 1
#define POLY_ELLIPSE 2
#define POLY_BONO 3
#define POLY_RYAN 4
#define POLY_CUBE 5

//도형을 그리고 나서 정보를 저장할 구조체
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
    //도형 범위 지정 및 반전 확인(큐브 뒤집기를 위해)
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
        //보노보노 뜬 눈 그리기
        angle = -140;
        hbrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, hbrush);
        for (int i = 0; i < 2; ++i) {
            double radian = (angle * i - 20) * M_PI / 180.0;
            int px = x + static_cast<int>(a * 4 / 5 * cos(radian));
            int py = y + static_cast<int>(b * 4 / 5 * sin(radian));
            Ellipse(hdc, px - a / 16, py - b / 16, px + a / 16, py + b / 16);
        }
    }
    else {
        //보노보노 감은 눈 그리기
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
    }
    //보노보노 입 그리기
    hbrush = CreateSolidBrush(RGB(255, 150, 255));
    SelectObject(hdc, hbrush);
    Ellipse(hdc, x - a / 5, y + b / 5, x + a / 5, b * 2 / 5 + y + b * 2 / 5);

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

void DrawRyan(HDC hdc, int x, int y, int sy, int ey, int a, int b) {
    bool isReversed = false; //드래그 방향에 따라 라이언 반전 여부 결정
    const int straightAngle = 180; //각도 180도
    if (sy > ey) {
        isReversed = true;
    }
    else {
        isReversed = false;
    }
    HBRUSH hbrush = CreateSolidBrush(RGB(255, 200, 15));
    HPEN hpen = CreatePen(BS_SOLID, 4, RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    SelectObject(hdc, hpen);
    //라이언 귀 그리기
    double angle = -80;
    for (int i = 0; i < 2; ++i) {
        double radian = (angle * i - 50) * M_PI / 180.0;
        if (isReversed) {
            radian = (angle * i - 50 - straightAngle) * M_PI / 180.0;
        }
        int px = x + static_cast<int>(a * cos(radian));
        int py = y + static_cast<int>(b * sin(radian));
        int ear_hRadius = a / 4;
        int ear_vRadius = b / 4;
        Ellipse(hdc, px - ear_hRadius, py - ear_vRadius, px + ear_hRadius, py + ear_vRadius);
    }
    //라이언 얼굴 그리기
    Ellipse(hdc, x - a, y - b, x + a, y + b);

    //라이언 눈 그리기
    DeleteObject(hpen);
    hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    angle = -140;
    for (int i = 0; i < 2; ++i) {
        double radian = (angle * i - 20) * M_PI / 180.0;
        if (isReversed) {
            radian = (angle * i - 20 - straightAngle) * M_PI / 180.0;
        }
        int px = x + static_cast<int>(a / 2 * cos(radian));
        int py = y + static_cast<int>(b / 2 * sin(radian));
        Ellipse(hdc, px - a / 12, py - b / 12, px + a / 12, py + b / 12);
        //라이언 눈썹 그리기
        if (isReversed) {
            Rectangle(hdc, px - a / 5, py + b * 1 / 3, px + a / 5, py + b * 1 / 3 + b / 12);
            int rds = b / 24;
            Ellipse(hdc, px - a / 5 - rds, py + b * 1 / 3, px - a / 5 + rds, py + b * 1 / 3 + b / 12);
            Ellipse(hdc, px + a / 5 - rds, py + b * 1 / 3, px + a / 5 + rds, py + b * 1 / 3 + b / 12);
        }
        else {
            Rectangle(hdc, px - a / 5, py - b * 1 / 3, px + a / 5, py - b * 1 / 3 + b / 12);
            int rds = b / 24;
            Ellipse(hdc, px - a / 5 - rds, py - b * 1 / 3, px - a / 5 + rds, py - b * 1 / 3 + b / 12);
            Ellipse(hdc, px + a / 5 - rds, py - b * 1 / 3, px + a / 5 + rds, py - b * 1 / 3 + b / 12);
        }
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
        if (isReversed) {
            radian = (angle * i + 70 - straightAngle) * M_PI / 180.0;
        }
        int px = x + static_cast<int>(a * 1 / 2 * cos(radian));
        int py = y + static_cast<int>(b * 1 / 2 * sin(radian));
        int snout_Hradius = a / 5;
        int snout_height = b * 2 / 5;
        if (isReversed) {
            Ellipse(hdc, px - snout_Hradius, y - b / 8, px + snout_Hradius, y - b / 8 - snout_height);
        }
        else {
            Ellipse(hdc, px - snout_Hradius, y + b / 8, px + snout_Hradius, y + b / 8 + snout_height);
        }
        points[i] = { px, py };
    }
    RECT rt;
    if (isReversed) {
        rt = { points[0].x, y - b / 4, points[1].x, y - b / 4 - b / 6 };
    }
    else {
        rt = { points[0].x, y + b / 4, points[1].x, y + b / 4 + b / 6 };
    }
    FillRect(hdc, &rt, hbrush);
    DeleteObject(hpen);
    //라이언 코 그리기
    hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hbrush);
    int nose_Hradius = a / 8;
    if (isReversed) {
        Ellipse(hdc, x - nose_Hradius, y - b / 16, x + nose_Hradius, y - b / 4);
    }
    else {
        Ellipse(hdc, x - nose_Hradius, y + b / 16, x + nose_Hradius, y + b / 4);
    }
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