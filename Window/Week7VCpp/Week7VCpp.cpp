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
    WCHAR name[10][KIND] = {L"펜/도형", L"타원", L"삼각형", L"직사각형", L"오각형", L"육각형", L"별", L"큐브"};
    const int margin_left = 10;
    const int margin_top = 10;
    const int button_width = margin_left + 50;
    const int button_height = margin_top + 30;
    int menu[KIND] = { BRUSH_SOLID, POLY_ELLIPSE, POLY_TRIANGLE, POLY_RECTANGLE, POLY_PENTAGON, POLY_HEXAGON, POLY_STAR, POLY_CUBE};
};
struct M_POLY {
    int width, height, Swidth, Sheight;
    void init(RECT crect, int selX, int selY) {
        width = crect.right - crect.left;
        height = crect.bottom - crect.top;
        Swidth = selX - crect.left;
        Sheight = selY - crect.top;
    };
};
M_POLY mpoly;
BUTTON btns{};

POINT startPoint{}, endPoint{}, selectPoint{};

RECT SortedRect{};

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
            case BRUSH_SOLID:
                isBrush = isBrush ? FALSE : TRUE;
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
        if(isEditing) {
        if (wParam == VK_DELETE) {
        
            }
        }
    case WM_KEYUP:
        if (wParam == VK_DELETE) {
            if (!(polygons.empty())) {
                polygons.pop_back();
                
                    WCHAR debugMessage[100];
                    wsprintf(debugMessage, L"ERASED\n");
                    OutputDebugString(debugMessage);
                endPoint.x = startPoint.x;
                endPoint.y = startPoint.y;
                isEditing = FALSE;
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
    //마우스 이벤트
    case WM_LBUTTONDOWN:
        //마우스 왼쪽 버튼 누름
    {
        if (!isDrawing)
        {
            if (isEditing) {
                selectPoint.x = LOWORD(lParam);
                selectPoint.y = HIWORD(lParam);
                bool polySelected = selectPoint.x >= startPoint.x && selectPoint.y >= startPoint.y && selectPoint.x <= endPoint.x && selectPoint.y <= endPoint.y;
                if (polySelected) {
                    endPoint.x = LOWORD(lParam);
                    endPoint.y = HIWORD(lParam);

                }
                else {
                    isEditing = FALSE;
                }
 
            }
            else {
                isDrawing = TRUE;
                startPoint.x = LOWORD(lParam);
                startPoint.y = HIWORD(lParam);
                WCHAR debugMessage[100];
                wsprintf(debugMessage, L"start: %d, %d\n", startPoint.x, startPoint.y);
                OutputDebugString(debugMessage);
            }
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

            if (startPoint.x != endPoint.x && startPoint.y != endPoint.y) {
                isAdding = TRUE;
            }
            WCHAR debugMessage[100];
            wsprintf(debugMessage, L"end: %d, %d\n", endPoint.x, endPoint.y);
            OutputDebugString(debugMessage);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        //다시 그리기
        if (isEditing) {
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);
            isEditing = FALSE;
            isAdding = TRUE;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        else {
            isAdding = TRUE;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        
        return 0;
    }
    case WM_RBUTTONUP:
    {   if (isEditing) {
            selectPoint.x = LOWORD(lParam);
            selectPoint.y = HIWORD(lParam);
            if (!isEditing) {
                isAdding = TRUE;
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }

    case WM_RBUTTONDOWN:
    {
        
        selectPoint.x = LOWORD(lParam);
        selectPoint.y = HIWORD(lParam);
        list<POLY>::reverse_iterator rit;
        POLY Selected{};
        int left, right, top, bottom;
        //if (!isEditing) {
            for (rit = polygons.rbegin(); rit != polygons.rend(); ++rit) {
                left = rit->rect.left;
                right = rit->rect.right;
                top = rit->rect.top;
                bottom = rit->rect.bottom;
                bool polySelected = selectPoint.x >= left && selectPoint.y >= top && selectPoint.x <= right && selectPoint.y <= bottom;
                if (polySelected) {
                    if (!isEditing) {
                        startPoint.x = left;
                        startPoint.y = top;
                        endPoint.x = right;
                        endPoint.y = bottom;
                        mpoly.init(rit->rect, selectPoint.x, selectPoint.y);
                        cPolygonState = rit->shape;
                    }
                    if (rit != polygons.rend()) {
                        if (!isEditing) {
                            polygons.erase(next(rit).base());
                            polygon_count -= 1;
                        }
                        else {
                            if (isEditing) {
                                
                            }
                        }
                        if (polygons.empty()) {
                            WCHAR debugMessage[100];
                            wsprintf(debugMessage, L"EMPTY\n");
                            OutputDebugString(debugMessage);
                        }
                    }
                    mpoly.Swidth = selectPoint.x - startPoint.x;
                    mpoly.Sheight = selectPoint.y - startPoint.y;
                    isEditing = true;
                    break;
                }
            }

        //}
    }
    case WM_MOUSEMOVE:
    {
            if (isDrawing)
            {   
                if (!isBrush) {
                    endPoint.x = LOWORD(lParam);
                    endPoint.y = HIWORD(lParam);
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                else {
                    startPoint.x = LOWORD(lParam);
                    startPoint.y = HIWORD(lParam);
                    endPoint.x = startPoint.x;
                    endPoint.y = startPoint.y;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
            if (isEditing) {
                if (wParam == MK_LBUTTON) {
                    endPoint.x = LOWORD(lParam);
                    endPoint.y = HIWORD(lParam);
                    InvalidateRect(hWnd, NULL, FALSE);
                }
                else if (wParam == MK_RBUTTON) {
                    selectPoint.x = LOWORD(lParam);
                    selectPoint.y = HIWORD(lParam);
                    startPoint.x = selectPoint.x - mpoly.Swidth;
                    startPoint.y = selectPoint.y - mpoly.Sheight;
                    endPoint.x = startPoint.x + mpoly.width;
                    endPoint.y = startPoint.y + mpoly.height;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
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
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 200, 255)); // 빨간색
        SelectObject(hdc, hBrush);
        //그렸던 브러쉬 그리기
        for (BRUSHES line : lines) {
            int left, right, top, bottom, cx, cy, h_r, v_r;
            left = line.point.x - line.b_radius;
            right = line.point.x + line.b_radius;
            top = line.point.y - line.b_radius;
            bottom = line.point.y + line.b_radius;
            cx = (left + right) / 2;
            cy = (top + bottom) / 2;
            h_r = (right - left) / 2;
            v_r = (bottom - top) / 2;
            switch (line.shape) {
            case POLY_ELLIPSE:
                Ellipse(hdc, left, top, right, bottom);
                break;
            case POLY_RECTANGLE:
                //직사각형 그리기
                Rectangle(hdc, left, top, right, bottom);
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
                //별 그리기(타원에 내접)
                DrawCube(hdc, left, top, right, bottom);
                break;
            }
        }


        //리스트에 저장된 도형 그리기
        list<POLY>::iterator it;
        for (it = polygons.begin(); it != polygons.end(); it++) {
            int left, right, top, bottom, cx, cy, h_r, v_r;
            left = it->rect.left;
            right = it->rect.right;
            top = it->rect.top;
            bottom = it->rect.bottom;
            cx = (left + right) / 2;
            cy = (top + bottom) / 2;
            h_r = (right - left) / 2;
            v_r = (bottom - top) / 2;

            switch (it->shape) {
            case POLY_ELLIPSE:
                Ellipse(hdc, left, top, right, bottom);
                break;
            case POLY_RECTANGLE:
                //직사각형 그리기
                Rectangle(hdc, left, top, right, bottom);
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
                //별 그리기(타원에 내접)
                DrawCube(hdc, left, top, right, bottom);
                break;
            }
        }


        h_radius = startPoint.x < endPoint.x ? (endPoint.x - startPoint.x) / 2 : (startPoint.x - endPoint.x) / 2;
        v_radius = startPoint.y < endPoint.y ? (endPoint.y - startPoint.y) / 2 : (startPoint.y - endPoint.y) / 2;



        // 왼쪽 쉬프트 누르고 있으면 도형의 가로 세로 비율 고정(1:1)
        if (GetKeyState(VK_LSHIFT) < 0) {
            h_radius = v_radius;
            if (startPoint.x < endPoint.x) {
                endPoint.x = startPoint.x + h_radius * 2;
                WCHAR debugMessage[100];
                wsprintf(debugMessage, L"이게 맞지\n");
                OutputDebugString(debugMessage);
            }
            else {
                endPoint.x = startPoint.x - h_radius * 2;
                WCHAR debugMessage[100];
                wsprintf(debugMessage, L"엔드가 더 작음\n");
                OutputDebugString(debugMessage);
            }
        }

        // 드래그 시작지점과 종료 지점을 정렬.
        //오른쪽 아래에서 왼쪽 위로 드래그한다면 시작점을 왼쪽 위로 지정하고 끝점을 오른쪽 아래로 지정.
        // 실제 포인트를 바꾸는 게 아니라 값을 RECT 객체에 저장해서 그릴 때만 사용.
        sortPoint(SortedRect, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        center.x = (SortedRect.left + SortedRect.right) / 2;
        center.y = (SortedRect.top + SortedRect.bottom) / 2;

        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        if (isBrush && isDrawing) {
            DrawBrush(hdc, endPoint.x, endPoint.y);
        }
        else if(!isBrush) {
            switch (cPolygonState) {
            case POLY_ELLIPSE:
                //타원 그리기
                Ellipse(hdc, SortedRect.left, SortedRect.top, SortedRect.right, SortedRect.bottom);
                break;
            case POLY_RECTANGLE:
                //직사각형 그리기
                Rectangle(hdc, SortedRect.left, SortedRect.top, SortedRect.right, SortedRect.bottom);
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
                //큐브 그리기(타원에 내접)
                DrawCube(hdc, SortedRect.left, SortedRect.top, SortedRect.right, SortedRect.bottom);
                break;
            }
        }
        DeleteObject(hBrush);
        if (isEditing) {
            sortPoint(SortedRect, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
            HPEN tmpPen = CreatePen(PS_DASH, 1, RGB(0, 0, 200));
            HBRUSH tmpBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            SelectObject(hdc, tmpPen);
            SelectObject(hdc, tmpBrush);
            Rectangle(hdc, SortedRect.left - 2, SortedRect.top - 2, SortedRect.right + 2, SortedRect.bottom + 2);
            DeleteObject(tmpPen);
            DeleteObject(tmpBrush);
        }
        if (isAdding == TRUE) {
            addPolyToList(SortedRect, cPolygonState);
            isAdding = FALSE;
        }

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
