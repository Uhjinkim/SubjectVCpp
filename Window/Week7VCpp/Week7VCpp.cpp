// Week7VCpp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Week7VCpp.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.



struct BUTTON {
    WCHAR name[10][KIND] = {L"타원", L"삼각형", L"직사각형", L"오각형", L"육각형", L"별", L"큐브"};
    const int margin_left = 10;
    const int margin_top = 10;
    const int button_width = margin_left + 50;
    const int button_height = margin_top + 30;
    int menu[KIND] = { POLY_ELLIPSE, POLY_TRIANGLE, POLY_RECTANGLE, POLY_PENTAGON, POLY_HEXAGON, POLY_STAR, POLY_CUBE};
};

BUTTON btns{};

POINT startPoint{}, endPoint{}, oldPoint{};
int cPolygonState = 0;

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
    LoadStringW(hInstance, IDC_WEEK7VCPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEEK7VCPP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEEK7VCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WEEK7VCPP);
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
   HWND hbuttons[KIND];
   for (int i = 0; i < KIND; i++) {
       hbuttons[i] = CreateWindowW(L"BUTTON", btns.name[i], WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
           btns.margin_left + (btns.margin_left + btns.button_width) * i, btns.margin_top, btns.button_width, btns.button_height, hWnd, (HMENU)btns.menu[i], hInst, nullptr);
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
   
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case POLY_ELLIPSE:
                cPolygonState = POLY_ELLIPSE;
                break;
            case POLY_TRIANGLE:
                cPolygonState = POLY_TRIANGLE;
                break;
            case POLY_RECTANGLE:
                cPolygonState = POLY_RECTANGLE;
                break;
            case POLY_PENTAGON:
                cPolygonState = POLY_PENTAGON;
                break;
            case POLY_HEXAGON:
                cPolygonState = POLY_HEXAGON;
                break;
            case POLY_STAR:
                cPolygonState = POLY_STAR;
                break;
            case POLY_CUBE:
                cPolygonState = POLY_CUBE;
                break;
           
                break;
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
        
    //마우스 이벤트
    case WM_LBUTTONDOWN:
    {
        if (!isDrawing)
        {
            isDrawing = TRUE;
            startPoint.x = LOWORD(lParam);
            startPoint.y = HIWORD(lParam); 
            WCHAR debugMessage[100];
            wsprintf(debugMessage, L"start: %d, %d\n",startPoint.x, startPoint.y);
            OutputDebugString(debugMessage);
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
            WCHAR debugMessage[100];
            wsprintf(debugMessage, L"end: %d, %d\n", endPoint.x, endPoint.y);
            OutputDebugString(debugMessage);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (isDrawing)
        {
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        return 0;
    }
    //
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        HDC tmpDC;
        GetClientRect(hWnd, &rect);

        HDC backBufferDC = CreateCompatibleDC(hdc);
        HBITMAP backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(backBufferDC, backBitmap);
        PatBlt(backBufferDC, 0, 0, rect.right, rect.bottom, WHITENESS);
        tmpDC = hdc;
        hdc = backBufferDC;
        backBufferDC = tmpDC;

        POINT center{};
        int h_radius, v_radius;
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 200, 255)); // 빨간색 박스
        SelectObject(hdc, hBrush);

        ////이전에 그린 별 다시 그리기
        //for (const STAR& star: stars) {
        //    Polygon(hdc, star.points, 10);
        //}
        ////이전에 그린 오각형 다시 그리기
        //for (const PENTA& penta : pentas) {
        //    Polygon(hdc, penta.points, 5);
        //}

        

        h_radius = startPoint.x < endPoint.x ? (endPoint.x - startPoint.x) / 2 : (startPoint.x - endPoint.x) / 2;
        v_radius = startPoint.y < endPoint.y ? (endPoint.y - startPoint.y) / 2 : (startPoint.y - endPoint.y) / 2;
       

        // 왼쪽 쉬프트 누르고 있으면 도형의 가로 세로 비율 고정(1:1)
        if (GetKeyState(VK_LSHIFT) < 0) {
            h_radius = v_radius;
            if (startPoint.x < endPoint.x) {
                endPoint.x = startPoint.x + h_radius * 2;
            }
            else {
                endPoint.x = startPoint.x - h_radius * 2;
            }
           }

        center.x = (startPoint.x + endPoint.x) / 2;
        center.y = (startPoint.y + endPoint.y) / 2;
 
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        switch (cPolygonState) {
            case POLY_ELLIPSE:
                //타원 그리기
                Ellipse(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                break;
            case POLY_RECTANGLE:
                //직사각형 그리기
                Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                break;
                //삼각형 그리기(타원에 내접)
            case POLY_TRIANGLE:
                DrawPolygons(hdc, center.x, center.y, h_radius, v_radius, 3);
                break;
            case POLY_PENTAGON:
                // 오각형 그리기(타원에 내접)
                DrawPolygons(hdc, center.x, center.y, h_radius, v_radius, 5);
                break;
            case POLY_HEXAGON:
                // 육각형 그리기(타원에 내접)
                DrawPolygons(hdc, center.x, center.y, h_radius, v_radius, 6);
                break;
            case POLY_STAR:
                //별 그리기(타원에 내접)
                DrawStar(hdc, center.x, center.y, h_radius, v_radius);
                break;
            case POLY_CUBE:
                //별 그리기(타원에 내접)
                DrawCube(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
                break;
        }
            DeleteObject(hBrush);

            tmpDC = hdc;
            hdc = backBufferDC;
            backBufferDC = tmpDC;
            GetClientRect(hWnd, &rect);
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, backBufferDC, 0, 0, SRCCOPY);
            SelectObject(backBufferDC, hOldBitmap);
            DeleteObject(backBitmap);
            DeleteDC(backBufferDC);
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
