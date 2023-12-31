﻿// WindowVCpp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowVCpp.h"

using namespace std;


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING] = L"202007017 김우진";                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
BOOL isDrawing = FALSE;
POINT startPoint{}, endPoint{}, oldPoint{};
POINT PentagonPoint[5];


struct POLY {
    int left;
    int top;
    int right;
    int bottom;
    WCHAR shape[MAX_LOADSTRING];
};
vector<POLY> polygons; // 그린 도형들을 저장하는 벡터


void DrawShapes(HDC hdc, int count)
{
    // 이전에 그린 사각형들을 백 버퍼에 그립니다.
    for (const POLY& poly : polygons)
    {
        if (wcscmp(poly.shape, L"Rectangle") == 0) {
            Rectangle(hdc, poly.left, poly.top, poly.right, poly.bottom);
        }
        else if (wcscmp(poly.shape, L"Ellipse") == 0) {
            Ellipse(hdc, poly.left, poly.top, poly.right, poly.bottom);
        }
        else if (wcscmp(poly.shape, L"Pentagon") == 0) {
            drawPolygon(hdc, poly.left, poly.top, poly.right, poly.bottom, 5);
        }

    }
    WCHAR debugMessage[100];
    wsprintf(debugMessage, L"함수 실행됨\n");
    OutputDebugString(debugMessage);
    
}

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_WINDOWVCPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWVCPP));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWVCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWVCPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   RECT rect = { 200, 200, 1000, 800 };
   AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
   int width = rect.right - rect.left;
   int height = rect.bottom - rect.top;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

   HWND hPolygonButton = CreateWindowW(L"BUTTON", L"도형: 사각형", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
       10, 10, 100, 30, hWnd, (HMENU)BTN_SET_POLYGON, hInst, nullptr);

   if (!hWnd)
   {
      return -1;
   }
   // 
   //
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   // 둘이 붙어다님
   //
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
    int wmId, wmEvent;
    HDC hdc{};
    RECT rect;
    RECT clientRect{};
    RECT textRect;
    HRGN bgRgn;
    HBRUSH hBrush{};
    HPEN hPen;
    static int count = 0;
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    HWND hPolygonButton{};


    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case BTN_SET_POLYGON:
            count++;
            if (count > 2) {
                count = 0;
            }
            switch (count) {
            case 0:
                SetDlgItemTextW(hWnd, BTN_SET_POLYGON, L"도형: 사각형");
                break;
            case 1:
                SetDlgItemTextW(hWnd, BTN_SET_POLYGON, L"도형: 타원");
                break;
            case 2:
                SetDlgItemTextW(hWnd, BTN_SET_POLYGON, L"도형: 오각형");
                break;
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        if (!isDrawing)
        {
            isDrawing = TRUE;
            startPoint.x = LOWORD(lParam);
            startPoint.y = HIWORD(lParam);
        }
        return 0;
    }

    case WM_LBUTTONUP:
    {
        if (isDrawing)
        {
            isDrawing = FALSE;
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);
            POLY poly;
            poly.left = startPoint.x;
            poly.top = startPoint.y;
            poly.right = endPoint.x;
            poly.bottom = endPoint.y;
            if (count == 0) {
                wcscpy_s(poly.shape, L"Rectangle");
            }
            else if (count == 1) {
                wcscpy_s(poly.shape, L"Ellipse");
            }
            else {
                wcscpy_s(poly.shape, L"Pentagon");
            }
            polygons.push_back(poly);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        if (isDrawing)
        {
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);
            RECT crect = {startPoint.x, startPoint.y, endPoint.x, endPoint.y};
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawShapes(hdc, count);
        if (isDrawing)
        {
            switch (count) {
            case 0:
                // 현재 드래그 중인 사각형을 그립니다.
                Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                break;
            case 1:
                Ellipse(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                break;

            case 2:
                drawPolygon(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y, 5);
                break;
            }
        }
        EndPaint(hWnd, &ps);
        return 0L;
    }
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return S_OK;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
