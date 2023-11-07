#pragma once

#include "resource.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <list>
#include <vector>
using namespace std;

#define MAX_POLYGON_EXIST 20 //저장되는 도형의 최대 개수

#define KIND 8

#define BRUSH_SOLID 0
#define BRUSH_ERASE 1

#define POLY_ELLIPSE 2
#define POLY_TRIANGLE 3
#define POLY_RECTANGLE 4
#define POLY_PENTAGON 5
#define POLY_HEXAGON 6
#define POLY_STAR 10
#define POLY_CUBE 12

BOOL isDrawing = FALSE;
BOOL isAdding = FALSE;
BOOL isEditing = FALSE;
BOOL isShiftKey = FALSE;
BOOL isBrush = TRUE;

int cPolygonState, cBrushState;

struct BRUSHES {
    POINT point{};
    int shape;
    int b_radius;
};
vector<BRUSHES> lines;

struct POLY {
    RECT rect;
    int shape;
};
list<POLY> polygons;

int polygon_count = 0; //그린 도형의 개수
void addPolyToList(RECT &rect, int shape) {
    POLY poly{};
    poly.rect = rect;
    poly.shape = shape;
    polygons.push_back(poly);
    polygon_count += 1;
    WCHAR debugMessage[100];
        wsprintf(debugMessage, L"ADDED: %d, total %d \n", polygons.back().shape, polygon_count);
        OutputDebugString(debugMessage);
}

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
//삼각형, 오각형, 육각형 그리기로 사용중
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

void DrawBrush(HDC hdc, int ex, int ey) {
    if (isBrush) {
        BRUSHES line;
        int left, right, top, bottom, cx, cy, h_r, v_r;
        line.point.x = ex;
        line.point.y = ey;
        line.b_radius = 10;
        left = line.point.x - line.b_radius;
        right = line.point.x + line.b_radius;
        top = line.point.y - line.b_radius;
        bottom = line.point.y + line.b_radius;
        cx = (left + right) / 2;
        cy = (top + bottom) / 2;
        h_r = (right - left) / 2;
        v_r = (bottom - top) / 2;

        switch (cPolygonState) {
        case POLY_ELLIPSE:
            Ellipse(hdc, left, right, top, bottom);
            break;
        case POLY_RECTANGLE:
            //직사각형 그리기
            Rectangle(hdc, left, right, top, bottom);
            break;
            //삼각형 그리기(타원에 내접)
        case POLY_TRIANGLE:
            DrawPolygons(hdc, cx, cy, h_r, v_r, 3);
            break;
        case POLY_PENTAGON:
            // 오각형 그리기(타원에 내접)
            DrawPolygons(hdc, cx, cy, h_r, v_r, 5);
            break;
        case POLY_HEXAGON:
            // 육각형 그리기(타원에 내접)
            DrawPolygons(hdc, cx, cy, h_r, v_r, 6);
            break;
        case POLY_STAR:
            //별 그리기(타원에 내접)
            DrawStar(hdc, cx, cy, h_r, v_r);
            break;
        case POLY_CUBE:
            //큐브 그리기(타원에 내접)
            DrawCube(hdc, left, right, top, bottom);
            break;
        }
        lines.push_back(line);
    }
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