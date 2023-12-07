// Ryan.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Ryan.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


POLY tmpPoly= {};
int polygon_state = POLY_RECTANGLE; // 현재 그리는 도형의 모양 지정: 초기값 사각형
BOOL onBonobono = FALSE;
BOOL isDrawing = FALSE; //현재 그리고 있는지
BOOL isEditing = FALSE; //크기조절이나 이동하고 있는지
BOOL isSpaceBarPressed = FALSE; //스페이스바 눌림 상태
POINT start{}, end{}, center{}, mPos{}, axis{}, grab{};
double dx, dy;
double angle;
int pWidth, pHeight;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LayoutProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CanvasProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RYAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RYAN));

    MSG msg;

    // 기본 메시지 루프입니다:
    while(GetMessage(&msg, nullptr, 0, 0)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RYAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   Widget mainWidget;
   mainWidget.setParent(nullptr);
   mainWidget.setClassName(szWindowClass);
   mainWidget.setWindowName(szTitle);
   mainWidget.dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX;
   mainWidget.width = VIEW_WIDTH;
   mainWidget.height = VIEW_HEIGHT;
   mainWidget.padding.setPadding(8);
   
   HWND hWnd = CreateWindowW(
       szWindowClass, 
       mainWidget.getWindowName(),
       mainWidget.dwStyle,
       100, 
       100, 
       VIEW_WIDTH, 
       VIEW_HEIGHT, 
       nullptr, 
       nullptr, 
       hInstance, 
       nullptr);
   if (!hWnd)
   {
       return FALSE;
   }
   
   RECT rcClient = { 0, 0, VIEW_WIDTH, VIEW_HEIGHT };
   AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);
   mainWidget.width = VIEW_WIDTH;
   mainWidget.height = VIEW_HEIGHT;
   MoveWindow(hWnd, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, FALSE);
   
   Widget layout;
       layout.setParent(&mainWidget);
       layout.setClassName(L"STATIC");
       layout.setWindowName(L"layout");
       layout.margin.setMargin(8);
       layout.padding.setPadding(8);
       layout.setStartPoint(0, 0);
       layout.setWidthAuto();
       layout.setHeightAuto();
       layout.dwStyle = WS_CHILD | WS_VISIBLE;

   HWND hWndLayout = CreateWindowExW(
       layout.dwExStyle,
       layout.getClassName(),
       layout.getWindowName(),
       layout.dwStyle,
       layout.x,
       layout.y,
       layout.width,
       layout.height,
       hWnd,
       nullptr,
       hInstance,
       nullptr
   );
   SetWindowLongPtr(hWndLayout, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(LayoutProc));
   if (!hWndLayout)
   {
      return FALSE;
   }
   
   
   Widget btnContainer;
        btnContainer.setParent(&layout);
        btnContainer.dwExStyle = WS_EX_TRANSPARENT;
        btnContainer.margin.setMargin(0);
        btnContainer.padding.setPadding(0);
        btnContainer.setStartPoint(0, 0);
        btnContainer.setWidthAuto();
        btnContainer.setHeight(64);
        btnContainer.setClassName(L"STATIC");
        btnContainer.setWindowName(L"btnContainer");
        btnContainer.dwStyle = WS_CHILD | WS_VISIBLE;
        HWND hWndBtnContainer = CreateWindowExW(
            btnContainer.dwExStyle,
            btnContainer.getClassName(),
            btnContainer.getWindowName(),
            btnContainer.dwStyle,
            btnContainer.x,
            btnContainer.y,
            btnContainer.width,
            btnContainer.height,
            hWndLayout,
            nullptr,
            hInstance,
            nullptr
            );
   
    ButtonWidget Button[BUTTON_COUNT];
    for (int i = 0; i < BUTTON_COUNT; i++) {
        Button[i].setParent(&btnContainer);
        Button[i].setClassName(L"BUTTON");
        Button[i].setStartPoint(Button[i].parent->width * i / BUTTON_COUNT, 0);
        Button[i].setWidth(Button[i].parent->width / BUTTON_COUNT - Button[i].margin.right - Button[i].margin.left);
    }
    
   
    Button[0].setWindowName(L"사각형");
    Button[0].hMenu = (HMENU)POLY_RECTANGLE;

    Button[1].setWindowName(L"타원");
    Button[1].hMenu = (HMENU)POLY_ELLIPSE;

    Button[2].setWindowName(L"보노보노");
    Button[2].hMenu = (HMENU)POLY_BONO;

    Button[3].setWindowName(L"라이언");
    Button[3].hMenu = (HMENU)POLY_RYAN;

    Button[4].setWindowName(L"큐브");
    Button[4].hMenu = (HMENU)POLY_CUBE;

    HWND hWndButtons[BUTTON_COUNT] = {};
    for (int i = 0; i < BUTTON_COUNT; i++) {
        hWndButtons[i] = CreateWindowExW(
            Button[i].dwExStyle, 
            Button[i].getClassName(), 
            Button[i].getWindowName(),
            Button[i].dwStyle,
            Button[i].x,
            Button[i].y, 
            Button[i].width,
            Button[i].height,
            hWndBtnContainer, 
            (HMENU)Button[i].hMenu, 
            hInstance,
            nullptr);
        if (!hWndButtons[i])
            return FALSE;
    }
    

    
    SetWindowLongPtr(hWndBtnContainer, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ButtonProc));
   
   Widget canvas;
       canvas.setParent(&layout);
       canvas.setClassName(L"STATIC");
       canvas.setWindowName(L"canvas");
       canvas.margin.setMargin(0);
       canvas.padding.setPadding(0);
       canvas.setStartPoint(0, btnContainer.height);
       canvas.setWidthAuto();
       canvas.setHeightAuto();
       canvas.dwStyle = WS_CHILD | WS_VISIBLE;

       HWND hWndCanvas = CreateWindowExW(
           canvas.dwExStyle,
           canvas.getClassName(),
           canvas.getWindowName(),
           canvas.dwStyle,
           canvas.x,
           canvas.y,
           canvas.width,
           canvas.height,
           hWndLayout,
           nullptr,
           hInstance,
           nullptr
        );

   if (!hWndCanvas)
   {
       return FALSE;
   }
    
   SetWindowLongPtr(hWndCanvas, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(CanvasProc));
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
            break;
    case WM_COMMAND:
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rect = { 0, 0, VIEW_WIDTH, VIEW_HEIGHT };
            HBRUSH b = CreateSolidBrush(RGB(255, 240, 200));
            SelectObject(hdc, b);
            FillRect(hdc, &rect, b);
            DeleteObject(b);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK LayoutProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect = {};
        GetClientRect(hWnd, &rect);
        HBRUSH b = CreateSolidBrush(RGB(255, 240, 200));
        SelectObject(hdc, b);
        HPEN p = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
        SelectObject(hdc, p);
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        DeleteObject(b);
        DeleteObject(p);
        EndPaint(hWnd, &ps);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK CanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{  
    static HDC hdc, hdcBuffer;
    static HBITMAP hBit, hOldBit;
    
    RECT cvs = {};
    switch (message)
    {
    case WM_CREATE:
        // 백 버퍼 생성
        GetClientRect(hWnd, &cvs);
        hdc = GetDC(hWnd);
        hBit = CreateCompatibleBitmap(hdc, cvs.right, cvs.bottom);
        hdcBuffer = CreateCompatibleDC(hdc);
        hOldBit = (HBITMAP) SelectObject(hdcBuffer, hBit);

        break;
    case WM_SETCURSOR:
        SetCursor(LoadCursor(nullptr, IDC_CROSS));
        break;
    case WM_COMMAND:
        SetFocus(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            isSpaceBarPressed = TRUE;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        break;
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            isSpaceBarPressed = FALSE;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        break;
    case WM_LBUTTONDOWN:
    {
        if (!isDrawing)
        {
            int left, right, top, bottom;
            left = tmpPoly.left;
            right = tmpPoly.right;
            top = tmpPoly.top;
            bottom = tmpPoly.bottom;
            bool polySelected = mPos.x >= left && mPos.y >= top && mPos.x <= right && mPos.y <= bottom;
            if (polySelected && tmpPoly.shape == POLY_CUBE) {
                if (!isEditing) {
                    start = { left, top };
                    end = { right, bottom };
                }
                axis.x = mPos.x;
                axis.y = mPos.y;
                pWidth = right - left;
                pHeight = bottom - top;
                angle = static_cast<double>(pHeight) / pWidth;
                isEditing = true;
                center = { getCenterX(start.x, end.x), getCenterY(start.y, end.y)};
                break;
            }
            else {
                isDrawing = TRUE;
                start.x = LOWORD(lParam);
                start.y = HIWORD(lParam);
                end.x = start.x;
                end.y = start.y;
            }
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (isDrawing)
        {
            isDrawing = FALSE;
            end.x = LOWORD(lParam);
            end.y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if (isEditing) {
            isEditing = FALSE;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        mPos.x = LOWORD(lParam);
        mPos.y = HIWORD(lParam);
        if (isDrawing)
        {
            end.x = LOWORD(lParam);
            end.y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if (isEditing) {
            if (wParam == MK_LBUTTON) {
                if (tmpPoly.shape == POLY_CUBE) {
                    if (mPos.x < axis.x) {
                        dx = axis.x - mPos.x;
                        pWidth = max(pWidth - pWidth * 0.005 * dx, 10);
                    }
                    else if (mPos.x > axis.x) {
                        dx = mPos.x - axis.x;
                        pWidth = max(pWidth + pWidth * 0.01 * dx, pWidth + pWidth * 0.01 * dx + 1);

                    }
                    pHeight = static_cast<double>(pWidth * angle);
                    start.x = center.x - pWidth / 2;
                    start.y = center.y - pHeight / 2;
                    end.x = center.x + pWidth / 2;
                    end.y = center.y + pHeight / 2;
                    if (tmpPoly.h_reverse) {
                        int tmpX = start.x;
                        start.x = end.x;
                        end.x = tmpX;
                    }
                    if (tmpPoly.v_reverse) {
                        int tmpY = start.y;
                        start.y = end.y;
                        end.y = tmpY;
                    }
                    axis.x = mPos.x;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
            if (wParam == MK_RBUTTON) {
                if (tmpPoly.shape == POLY_RECTANGLE) {
                    start.x = mPos.x - grab.x;
                    start.y = mPos.y - grab.y;
                    end.x = start.x + pWidth;
                    end.y = start.y + pHeight;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                else if (tmpPoly.shape == POLY_ELLIPSE) {
                    if (mPos.x < axis.x) {
                        dx = axis.x - mPos.x;
                        pWidth = max(pWidth - pWidth * 0.005 * dx, 10);
                    }
                    else if (mPos.x > axis.x) {
                        dx = mPos.x - axis.x;
                        pWidth = max(pWidth + pWidth * 0.01 * dx, pWidth + pWidth * 0.01 * dx + 1);

                    }
                    pHeight = static_cast<double>(pWidth * angle);
                    start.x = center.x - pWidth / 2;
                    start.y = center.y - pHeight / 2;
                    end.x = center.x + pWidth / 2;
                    end.y = center.y + pHeight / 2;
                    axis.x = mPos.x;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                else if (tmpPoly.shape == POLY_CUBE) {
                    start.x = mPos.x - grab.x;
                    start.y = mPos.y - grab.y;
                    end.x = start.x + pWidth;
                    end.y = start.y + pHeight;
                    if (tmpPoly.h_reverse) {
                        int tmpX = start.x;
                        start.x = end.x;
                        end.x = tmpX;
                    }
                    if (tmpPoly.v_reverse) {
                        int tmpY = start.y;
                        start.y = end.y;
                        end.y = tmpY;
                    }
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
        }
        break;
    }
    case WM_RBUTTONDOWN:
    {
        int left, right, top, bottom;
        left = tmpPoly.left;
        right = tmpPoly.right;
        top = tmpPoly.top;
        bottom = tmpPoly.bottom;
        bool polySelected = mPos.x >= left && mPos.y >= top && mPos.x <= right && mPos.y <= bottom;
        if (polySelected) {
            if (!isEditing) {
                start.x = left;
                start.y = top;
                end.x = right;
                end.y = bottom;
            }
            axis.x = mPos.x;
            axis.y = mPos.y;
            grab.x = mPos.x - left;
            grab.y = mPos.y - top;
            pWidth = right - left;
            pHeight = bottom - top;
            angle = static_cast<double>(pHeight) / pWidth; // 가로 길이 세로 길이 비율 저장
            isEditing = TRUE;
            center = { (start.x + end.x) / 2, (start.y + end.y) / 2 };
            break;
        }
        //}
    }
    case WM_RBUTTONUP:
    {   if (isEditing) {
        isEditing = FALSE;
        InvalidateRect(hWnd, NULL, FALSE);
    }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        //백버퍼 생성하고 배경 그리기
        RECT rect = {};
        GetClientRect(hWnd, &rect);
        hBit = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        hdcBuffer = CreateCompatibleDC(hdc);
        hOldBit = (HBITMAP)SelectObject(hdcBuffer, hBit);
        HBRUSH bgBrush = CreateSolidBrush(RGB(255, 255, 255));
        SelectObject(hdcBuffer, bgBrush);
        HPEN borderPen = CreatePen(BS_SOLID, 1, RGB(0, 0, 0));
        SelectObject(hdcBuffer, borderPen);
        Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
        DeleteObject(bgBrush);
       
        HBRUSH hBrush;
        center = { getCenterX(start.x, end.x), getCenterY(start.y, end.y) };
        int h_radius = getRadiusHorizontal(start.x, end.x);
        int v_radius = getRadiusVertical(start.y, end.y);
        switch (polygon_state) {
        case POLY_RECTANGLE:
            hBrush = CreateSolidBrush(RGB(171, 235, 52));
            SelectObject(hdcBuffer, hBrush);
            Rectangle(hdcBuffer, start.x, start.y, end.x, end.y);
            DeleteObject(hBrush);
            savePoly(tmpPoly, start.x, start.y, end.x, end.y, POLY_RECTANGLE);
            break;
        case POLY_ELLIPSE:
            hBrush = CreateSolidBrush(RGB(243, 166, 255));
            SelectObject(hdcBuffer, hBrush);
            Ellipse(hdcBuffer, start.x, start.y, end.x, end.y);
            DeleteObject(hBrush);
            savePoly(tmpPoly, start.x, start.y, end.x, end.y, POLY_ELLIPSE);
            break;
        case POLY_RYAN:
            DrawRyan(hdcBuffer, center.x, center.y, start.y, end.y, h_radius, v_radius);
            break;
        case POLY_CUBE:
            hBrush = CreateSolidBrush(RGB(130, 94, 219));
            SelectObject(hdcBuffer, hBrush);
            DrawCube(hdcBuffer, start.x, start.y, end.x, end.y);
            DeleteObject(hBrush);
            savePoly(tmpPoly, start.x, start.y, end.x, end.y, POLY_CUBE);
            break;
        }
        if (polygon_state == POLY_BONO && onBonobono) {
            center = {getCenterX(rect.left, rect.right), getCenterY(rect.top, rect.bottom)};
            h_radius = 100;
            v_radius = 100;
            DrawBonobono(hdcBuffer, center.x, center.y, h_radius, v_radius, isSpaceBarPressed);
        }
        POINT border[] = {
            {rect.left, rect.top},
            {rect.right-1, rect.top},
            {rect.right-1, rect.bottom-1},
            {rect.left, rect.bottom-1},
            {rect.left, rect.top}
        };
        SelectObject(hdcBuffer, borderPen);
        Polyline(hdcBuffer, border, 5);
        DeleteObject(borderPen);
        BitBlt(hdc, rect.left, rect.top, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY);
        SelectObject(hdcBuffer, hOldBit);
        
        DeleteDC(hdcBuffer);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        if (hBit) {
            DeleteObject(hBit);
        }
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case POLY_RECTANGLE:
            polygon_state = POLY_RECTANGLE;
            tmpPoly = {};
            start = {};
            end = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_ELLIPSE:
            polygon_state = POLY_ELLIPSE;
            tmpPoly = {};
            start = {};
            end = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_BONO:
            if (polygon_state == POLY_BONO)
                onBonobono = !onBonobono;
            else {
                polygon_state = POLY_BONO;
                onBonobono = TRUE;
            }
            tmpPoly = {};
            start = {};
            end = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_RYAN:
            polygon_state = POLY_RYAN;
            tmpPoly = {};
            start = {};
            end = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_CUBE:
            polygon_state = POLY_CUBE;
            tmpPoly = {};
            start = {};
            end = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        SetFocus(hWnd);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect = {};
        GetClientRect(hWnd, &rect);
        HBRUSH b = CreateSolidBrush(RGB(255, 240, 200));
        FillRect(hdc, &rect, b);
        EndPaint(hWnd, &ps);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
