#pragma once

#include "resource.h"
#include <math.h>
#include <vector>


#define MAX_LOADSTRING 100
#define BTN_SET_POLYGON 1001
#define PI 3.141592f

void drawPolygon(HDC hdc, int x1, int y1, int x2, int y2, int vertex) {
    POINT center = { x2 - (x2-x1)/2, y2 - (y2 - y1) / 2 };
    int radius = (x2 - x1) / 2;
    float angle = 360.0f / vertex;
    POINT vertexs[5];
    MoveToEx(hdc, center.x, center.y, NULL);
    for (int i = 0; i < vertex; i++) {
        float nextAngle = (PI * angle * i) / 180.0f;
        center.x = radius * cosf(nextAngle) + center.x;
        center.y = radius * sinf(nextAngle) + center.y;
        vertexs[i] = { center.x, center.y };

        LineTo(hdc, center.x, center.y);
    }
    for (int i = 0; i < 5; i++) {
        MoveToEx(hdc, vertexs[i].x, vertexs[i].y, NULL);
        int opt1 = i == 4 ? -1 : i + 2;
        int opt2 = i == 4 ? -1 : i + 3;
        LineTo(hdc, vertexs[opt1].x, vertexs[opt1].y);
        LineTo(hdc, vertexs[opt2].x, vertexs[opt2].y);

    }
}