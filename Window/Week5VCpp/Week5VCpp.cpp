// Week5VCpp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Week5VCpp.h"

#define MAX_LOADSTRING 100


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
bool isDrawing = FALSE;
bool isDragging = FALSE;
POINT startPoint{}, endPoint{}, movePoint{}, StoM{}, StoE{};
int count = 0;
struct POLY {
    int left;
    int top;
    int right;
    int bottom;
    WCHAR shape[MAX_LOADSTRING];
};
POLY poly{};
void sortPoint(POLY &poly, int sx, int sy, int ex, int ey) {
    if (sx > ex) {
        poly.left = ex;
        poly.right = sx;
    }
    else {
        poly.left = sx;
        poly.right = ex;
    }
    if (sy > ey) {
        poly.top = ey;
        poly.bottom = sy;
    }
    else {
        poly.top = sy;
        poly.bottom = ey;
    }
    
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
    LoadStringW(hInstance, IDC_WEEK5VCPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEEK5VCPP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEEK5VCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WEEK5VCPP);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
    RECT clientRect{};
    HWND hPolygonButton{};
    switch (message)
    {
    case WM_CREATE:
    {
        break;
    }
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
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        if (!isDrawing)
        {
            movePoint.x = LOWORD(lParam);
            movePoint.y = HIWORD(lParam);
            if (movePoint.x >= poly.left && movePoint.x <= poly.right && movePoint.y >= poly.top && movePoint.y <= poly.bottom) {
                isDragging = TRUE;
                startPoint.x = poly.left;
                startPoint.y = poly.top;
                endPoint.x = poly.right;
                endPoint.y = poly.bottom;
                StoM.x = movePoint.x - startPoint.x;
                StoM.y = movePoint.y - startPoint.y;
                WCHAR debugMessage[100];
                wsprintf(debugMessage, L"start: %d, %d / end: %d, %d\n", poly.left, poly.top, poly.right, poly.bottom);
                OutputDebugString(debugMessage);
                break;
            }
            isDrawing = TRUE;
            startPoint.x = LOWORD(lParam);
            startPoint.y = HIWORD(lParam);
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {   if (isDragging) {
        isDragging = FALSE;
        poly.left = startPoint.x;
        poly.top = startPoint.y;
        poly.right = endPoint.x;
        poly.bottom = endPoint.y;
        wcscpy_s(poly.shape, L"Rectangle");
        sortPoint(poly, poly.left, poly.top, poly.right, poly.bottom);
    }
        if (isDrawing)
        {
            isDrawing = FALSE;
            poly.left = startPoint.x;
            poly.top = startPoint.y;
            poly.right = endPoint.x;
            poly.bottom = endPoint.y;
            wcscpy_s(poly.shape, L"Rectangle");
            sortPoint(poly, poly.left, poly.top, poly.right, poly.bottom);
            StoE.x = poly.right - poly.left;
            StoE.y = poly.bottom - poly.top;
            WCHAR debugMessage[100];
            wsprintf(debugMessage, L"start: %d, %d / end: %d, %d\n", poly.left, poly.top, poly.right, poly.bottom);
            OutputDebugString(debugMessage);

        }
        return 0L;
    }
    case WM_MOUSEMOVE:
    {
        if (isDragging) {
            movePoint.x = LOWORD(lParam);
            movePoint.y = HIWORD(lParam);
            WCHAR debugMessage[100];
            wsprintf(debugMessage, L"moving: %d, %d\n", movePoint.x, movePoint.y);
            OutputDebugString(debugMessage);
            startPoint.x = movePoint.x - StoM.x;
            startPoint.y = movePoint.y - StoM.y;
            endPoint.x = startPoint.x + StoE.x;
            endPoint.y = startPoint.y + StoE.y;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (isDrawing)
        {
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);

            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (isDragging) {
            //그렸던 사각형을 옮겨 그립니다.
            Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        }
        if (isDrawing)
        {
                // 현재 드래그 중인 사각형을 그립니다.
                Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);

            EndPaint(hWnd, &ps);
        }
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
