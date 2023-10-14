// Week6VCpp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Week6VCpp.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
POINT endPoint = { 0 };
int isKeyPressed = 0;
RECT clientRect{};
RECT rect_user = { 5, 5, 10, 10 }; // 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형
RECT rect_target = { 50, 50, 150, 150 }; // 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형

bool movingRight = false;
bool movingLeft = false;
bool movingUp = false;
bool movingDown = false;

UINT_PTR timerID;

POINT offset = { 0,0 };

void moveUser(HWND hWnd) {
    
        if (movingRight == movingLeft || (rect_user.left <= clientRect.left && movingLeft) || (rect_user.right >= clientRect.right && movingRight)) {
            offset.x = 0;
        }
        else if (movingLeft) {
            offset.x = -5;
        }
        else if (movingRight) {
            offset.x = 5;
        }
        if (movingUp == movingDown || (rect_user.top <= clientRect.top && movingUp) || (rect_user.bottom >= clientRect.bottom && movingDown)) {
            offset.y = 0;
        }
        else if (movingDown) {
            offset.y = 5;
        }
        else if (movingUp) {
            offset.y = -5;
        }
        rect_user.left += offset.x;
        rect_user.right += offset.x;
        rect_user.top += offset.y;
        rect_user.bottom += offset.y;
        OutputDebugString(L"Moving..\n");
        InvalidateRect(hWnd, NULL, TRUE);
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
    // 


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WEEK6VCPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEEK6VCPP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEEK6VCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WEEK6VCPP);
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
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

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
    switch (message)
    {
    case WM_CREATE:
        // 타이머 생성
        timerID = SetTimer(hWnd, 1, 16, NULL); // 16ms 간격으로 타이머 실행 (약 60fps)
        break;
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
    case WM_KEYDOWN:
        
            if (wParam == VK_RIGHT) {
                movingRight = TRUE;
            }
            if (wParam == VK_LEFT) {
                movingLeft = TRUE;
            }
            if (wParam == VK_UP) {
                movingUp = TRUE;
            }
            if (wParam == VK_DOWN) {
                movingDown = TRUE;
            }
        break;
    case WM_KEYUP:
        if (wParam == VK_RIGHT) {
            movingRight = FALSE;
        }
        if (wParam == VK_LEFT) {
            movingLeft = FALSE;
        }
        if (wParam == VK_UP) {
            movingUp = FALSE;
        }
        if (wParam == VK_DOWN) {
            movingDown = FALSE;
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &clientRect);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255));
            HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255));
            HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &rect_target, hBrush_target);
            FillRect(hdc, &rect_user, hBrush_user);
            bool isStartPointOver = rect_user.left >= rect_target.left && rect_user.top >= rect_target.top;
            bool isEndPointOver = rect_user.right <= rect_target.right && rect_user.bottom <= rect_target.bottom;
            if (isStartPointOver && isEndPointOver) {
                TextOutW(hdc, 300, 200, L"CRASH!!!", 8);
            }
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        moveUser(hWnd);
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
