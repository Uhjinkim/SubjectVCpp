#pragma once

#include "resource.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <list>
using namespace std;

#define MAX_POLYGON_EXIST 20 //저장되는 도형의 최대 개수

#define KIND 7

#define POLY_RECTANGLE 1
#define POLY_ELLIPSE 2
#define POLY_TRIANGLE 3
#define POLY_PENTAGON 5
#define POLY_HEXAGON 6
#define POLY_STAR 10
#define POLY_CUBE 12

BOOL isDrawing = FALSE;
BOOL isShiftKey = FALSE;
struct POLY {

};

//int polygon_count = 0; //그린 도형의 개수. 색인 지정
//struct TRI : public POLY{
//    POINT points[3];
//};
//struct STAR : public POLY {
//    POINT points[10];
//};
//struct PENTA : public POLY {
//    POINT points[5];
//};
//
//list<TRI> tris;
//list<STAR> stars;
//list<PENTA> pentas;


void sortPoint(RECT &rect, int sx, int sy, int ex, int ey) {
    if (sx > ex) {
        rect.left = ex;
        rect.right = sx;
    }
    else {
        rect.left = sx;
        rect.right = ex;
    }
    if (sy > ey) {
        rect.top = ey;
        rect.bottom = sy;
    }
    else {
        rect.top = sy;
        rect.bottom = ey;
    }

}

//DrawPolygons(x, y는 기준 타원의 원점 좌표, a는 가로 반지름, b는 세로 반지름, vertex는 꼭짓점 개수)

void DrawPolygons(HDC hdc, int x, int y, int a, int b, int vertex) {
    POINT points[10];
    double angle = 360.0 / vertex;

    for (int i = 0; i < vertex; i++) {
        double radian = (angle * i - 90) * M_PI / 180.0;
        int px = x + static_cast<int>(a * cos(radian));
        int py = y + static_cast<int>(b * sin(radian));
        points[i] = { px, py };
    }
    Polygon(hdc, points, vertex);
}

//삼각형 그리기
void DrawTriangle(HDC hdc, int x, int y, int a, int b) {
    POINT points[3];
    double angle = 360.0 / 3;

    for (int i = 0; i < 3; i++) {
        double radian = (angle * i - 90) * M_PI / 180.0;
        int px = x + static_cast<int>(a * cos(radian));
        int py = y + static_cast<int>(b * sin(radian));
        points[i] = { px, py };
    }

    Polygon(hdc, points, 3);

 /*   if (isDrawing == FALSE) {
        TRI tri;
        for (int i = 0; i < 3; i++) {
            tri.points[i] = points[i];
        }
        tris.push_back(tri);
    }*/
}

//오각형 그리기
void DrawPentagon(HDC hdc, int x, int y, int a, int b) {
    POINT points[5];
    double angle = 360.0 / 5;

    for (int i = 0; i < 5; i++) {
        double radian = (angle * i - 90) * M_PI / 180.0;
        int px = x + static_cast<int>(a * cos(radian));
        int py = y + static_cast<int>(b * sin(radian));
        points[i] = { px, py };
        WCHAR debugMessage[100];
        wsprintf(debugMessage, L"point: %d, %d\n", px, py);
        OutputDebugString(debugMessage);
    }

    Polygon(hdc, points, 5);

   /* if (isDrawing == FALSE) {
        PENTA penta;
        for (int i = 0; i < 5; i++) {
            penta.points[i] = points[i];
        }
        pentas.push_back(penta);
    }*/
}
//별 그리기
void DrawStar(HDC hdc, int x, int y, int a, int b) {
    POINT points[10];
    double angle = 360.0 / 5;
    int px, py;

    for (int i = 0; i < 5; i++) {
        double outer_radian = (angle * i - 90) * M_PI / 180.0;
        px = x + static_cast<int>(a * cos(outer_radian));
        py = y + static_cast<int>(b * sin(outer_radian));
        points[i * 2] = { px, py };
    }
    for(int i = 0; i < 5; i++) {
        double inner_radian = (angle * i + 90) * M_PI / 180.0;
        px = x + static_cast<int>(a/2 * cos(inner_radian));
        py = y + static_cast<int>(b/2 * sin(inner_radian));
        points[i<3 ? i * 2 + 5 : i*2 -5] = { px, py };
    }
    // 별모양 그리기
    Polygon(hdc, points, 10);

    /*if (isDrawing == FALSE) {
        STAR star;
        for (int i = 0; i < 10; i++) {
            star.points[i] = points[i];
        }
        stars.push_back(star);
    }*/
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
/*
void DrawHeart(HDC hdc,int cx, int cy, int a, int b) {
    int numpoints = 5000;
    double radius, theta, x, y, h_rad, v_rad;
    for (int i = 0; i < numpoints; ++i) {
        theta = 2.0 * M_PI * i / numpoints;
        h_rad = a * (1.0 - sin(theta));
        v_rad = b * (1.0 - sin(theta));
        x = cx + h_rad * cos(theta);
        y = cy - v_rad * sin(theta);

        SetPixel(hdc, static_cast<int>(x), static_cast<int>(y), RGB(255, 0, 0)); // 빨간색 픽셀 그리기
    }
}
*/