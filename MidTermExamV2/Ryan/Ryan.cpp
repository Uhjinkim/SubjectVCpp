// Ryan.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Ryan.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


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
   mainWidget.setWindowName(szTitle);
   mainWidget.dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX & ~WS_THICKFRAME;
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
   MoveWindow(hWnd, 100, 100, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, FALSE);

   Widget layout;
       layout.setWindowName(L"layout");
       layout.margin.setMargin(8);
       layout.padding.setPadding(8);
       layout.dwStyle = WS_CHILD | WS_VISIBLE;
       layout.width = mainWidget.width - layout.margin.right - mainWidget.padding.right;
       layout.height = mainWidget.height - layout.margin.bottom - mainWidget.padding.bottom;

   HWND hWndLayout = CreateWindowExW(
       layout.dwExStyle,
       L"STATIC",
       layout.getWindowName(),
       layout.dwStyle,
       layout.margin.left + mainWidget.padding.left, 
       layout.margin.top + mainWidget.padding.top, 
       layout.width,
       layout.height,
       hWnd,
       nullptr,
       hInstance,
       nullptr);

   if (!hWndLayout)
   {
      return FALSE;
   }

   SetWindowLongPtr(hWndLayout, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(LayoutProc));

   Widget btnContainer;
    btnContainer.setWindowName(L"btnContainer");
    btnContainer.margin.setMargin(0);
    btnContainer.padding.setPadding(0);
    btnContainer.dwStyle = WS_CHILD;
    btnContainer.width = layout.width - btnContainer.margin.right - layout.padding.right;
    btnContainer.height = 64 - btnContainer.margin.bottom;
    HWND hWndBtnContainer = CreateWindowExW(
        btnContainer.dwExStyle,
        L"STATIC",
        btnContainer.getWindowName(),
        btnContainer.dwStyle,
        btnContainer.margin.left + layout.padding.left,
        btnContainer.margin.top + layout.padding.top,
        btnContainer.width,
        btnContainer.height,
        hWndLayout,
        nullptr,
        hInstance,
        nullptr);
   
    ButtonWidget ButtonEllipse;
    ButtonWidget ButtonRectangle;
    ButtonWidget ButtonBonobono;
    ButtonWidget ButtonRyan;
    ButtonWidget ButtonCube;

    ButtonEllipse.setWindowName(L"타원");
    ButtonEllipse.menu = POLY_ELLIPSE;

    ButtonRectangle.setWindowName(L"사각형");
    ButtonRectangle.menu = POLY_RECTANGLE;

    ButtonBonobono.setWindowName(L"보노보노");
    ButtonBonobono.menu = POLY_BONO;

    ButtonRyan.setWindowName(L"라이언");
    ButtonRyan.menu = POLY_RYAN;

    ButtonCube.setWindowName(L"큐브");
    ButtonCube.menu = POLY_CUBE;
    //ButtonInfo btn;
    /*
    HWND hWndButtons[BUTTON_COUNT];
    for (int i = 0; i < BUTTON_COUNT; i++) {
        hWndButtons[i] = CreateWindowExW(
            Buttons[i].dwExStyle, 
            Buttons[i].getClassName(), 
            Buttons[i].getWindowName(),
            Buttons[i].dwStyle,
            ((Buttons[i].margin.left + btnContainer.width / BUTTON_COUNT) * i), 
            Buttons[i].margin.top, 
            btn.width, 
            btn.height, 
            hWndBtnContainer, 
            (HMENU)Buttons[i].menu, 
            hInstance,
            nullptr);
    }
    */


    SetWindowLongPtr(hWndBtnContainer, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ButtonProc));

   Widget canvas;
       canvas.setWindowName(L"canvas");
       canvas.margin.setMargin(0);
       canvas.padding.setPadding(0);
       canvas.dwStyle = WS_CHILD | WS_VISIBLE;

   HWND hWndCanvas = CreateWindowExW(
       canvas.dwExStyle,
       L"STATIC",
       canvas.getWindowName(),
       canvas.dwStyle,
       canvas.margin.left + layout.padding.left,
       canvas.margin.top + layout.padding.top + btnContainer.height,
       layout.width - canvas.margin.right - layout.padding.right,
       layout.height - btnContainer.height - canvas.margin.bottom - layout.padding.bottom,
       hWndLayout,
       nullptr,
       hInstance,
       nullptr);

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
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
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
        HBRUSH b = CreateSolidBrush(RGB(255, 200, 200));
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
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect = {};
        GetClientRect(hWnd, &rect);
        HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
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

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {   
        
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect = {};
        GetClientRect(hWnd, &rect);
        HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
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
