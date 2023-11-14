// Ryan.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "yuhanCG.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
RECT rcClient = { 0, 0, VIEW_WIDTH, VIEW_HEIGHT }; //클라이언트 영역 지정 위한 사각형
RECT client, layout, canvas{}; // 큰 박스는 레이아웃, 드로잉영역은 캔버스라고 지정
HWND hbuttons[BUTTON_COUNT]; //반복문 사용해 버튼 생성
int polygon_state = POLY_RECTANGLE; // 현재 그리는 도형의 모양 지정: 초기값 사각형

//레이아웃과 캔버스 마진
const int margin = 8;
const int padding = 8;

bool isMouseInCanvas; //현재 마우스 커서가 드로잉 영역 안에 있는지
bool onBonobono = false;

//반복문 사용하여 버튼 5개를 생성하기 위한 구조체
struct BUTTON {
    WCHAR name[BUTTON_COUNT][10] = { L"직사각형", L"타원", L"보노보노", L"라이언", L"큐브" };
    int width = DFBUTTON_WIDTH;
    int height = DFBUTTON_HEIGHT;
    int margin = 16; //버튼의 마진
    int menu[BUTTON_COUNT] = { POLY_RECTANGLE, POLY_ELLIPSE, POLY_BONO, POLY_RYAN, POLY_CUBE };
};

BUTTON btn{};
POLY tmpPoly{};
POINT start, end, mPos, axis, center, grab{};
BOOL isDrawing = FALSE; //현재 그리고 있는지
BOOL isEditing = FALSE; //크기조절이나 이동하고 있는지
BOOL isSpaceBarPressed = FALSE;

double dx, dy;
double angle;
int pWidth, pHeight;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_MTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }


    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MTE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

    AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX & ~WS_THICKFRAME,
        100, 100, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, nullptr, nullptr, hInstance, nullptr);

    GetClientRect(hWnd, &client);
    layout = { client.left + margin, client.top + margin, client.right - padding, client.bottom - padding };
    canvas = { layout.left + margin, layout.top + btn.margin * 2 + btn.height, layout.right - padding, layout.bottom - padding };




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
    {
        for (int i = 0; i < BUTTON_COUNT; i++) {
            hbuttons[i] = CreateWindowW(L"BUTTON", btn.name[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                margin * 2 + ((margin + btn.width) * i), margin * 2, btn.width, btn.height, hWnd, (HMENU)btn.menu[i], hInst, nullptr);
        }
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case POLY_RECTANGLE:
            polygon_state = 1;
            tmpPoly = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_ELLIPSE:
            polygon_state = 2;
            tmpPoly = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_BONO:
            polygon_state = 3;
            tmpPoly = {};
            onBonobono = !onBonobono;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_RYAN:
            polygon_state = 4;
            tmpPoly = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case POLY_CUBE:
            polygon_state = 5;
            tmpPoly = {};
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        SetFocus(hWnd);
    }
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
                    start.x = left;
                    start.y = top;
                    end.x = right;
                    end.y = bottom;

                }
                axis.x = mPos.x;
                axis.y = mPos.y;
                pWidth = right - left;
                pHeight = bottom - top;
                angle = static_cast<double>(pHeight) / pWidth;
                isEditing = true;
                center = { (start.x + end.x) / 2, (start.y + end.y) / 2 };
                break;
            }
            else {
                isDrawing = TRUE;
                start.x = LOWORD(lParam);
                start.y = HIWORD(lParam);
                WCHAR debugMessage[100];
                wsprintf(debugMessage, L"start: %d, %d\n", start.x, start.y);
                OutputDebugString(debugMessage);
            }
        }
        return 0;
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

    case WM_LBUTTONUP:
    {
        if (isDrawing)
        {
            isDrawing = FALSE;
            end.x = LOWORD(lParam);
            end.y = HIWORD(lParam);
            WCHAR debugMessage[100];
            wsprintf(debugMessage, L"end: %d, %d\n", end.x, end.y);
            OutputDebugString(debugMessage);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if (isEditing) {
            isEditing = FALSE;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        mPos.x = LOWORD(lParam);
        mPos.y = HIWORD(lParam);
        isMouseInCanvas = mPos.x > canvas.left && mPos.y > canvas.top && mPos.x < canvas.right && mPos.y < canvas.bottom;
        if (isMouseInCanvas) {
            SetCursor(LoadCursor(nullptr, IDC_CROSS));
        }
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
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        POINT center{};
        HBRUSH hbrush;
        int h_radius, v_radius;
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        GetClientRect(hWnd, &client);
        hbrush = CreateSolidBrush(RGB(255, 240, 200));
        SelectObject(hdc, hbrush);
        FillRect(hdc, &client, hbrush); //배경 채우기
        //버튼 크기. 위치 조정
        btn.width = (layout.right - layout.left) / BUTTON_COUNT - btn.margin * 2;
        for (int i = 0; i < BUTTON_COUNT; i++) {
            SetWindowPos(hbuttons[i], NULL, layout.left + (layout.right - layout.left) / BUTTON_COUNT * (i)+btn.margin, layout.top + btn.margin, btn.width, btn.height, SWP_NOZORDER);
        }

        //컨테이너 박스, 드로잉영역 그리기
        Rectangle(hdc, layout.left, layout.top, layout.right, layout.bottom);
        Rectangle(hdc, canvas.left, canvas.top, canvas.right, canvas.bottom);
        DeleteObject(hbrush);

        //스페이스바 눌렸는지 체크
        if (GetKeyState(VK_SPACE) < 0) {
            isSpaceBarPressed = TRUE;
        }
        else {
            isSpaceBarPressed = FALSE;
        }

        //드래그 시작점과 끝점의 중심
        center.x = (start.x + end.x) / 2;
        center.y = (start.y + end.y) / 2;

        h_radius = start.x < end.x ? (end.x - start.x) / 2 : (start.x - end.x) / 2; //가로 길이의 반
        v_radius = start.y < end.y ? (end.y - start.y) / 2 : (start.y - end.y) / 2; //세로 길이의 반
        bool isStartInCanvas = start.x > canvas.left && start.y > canvas.top && start.x < canvas.right && start.y < canvas.bottom; //시작점이 캔버스 안에 있는지
        bool isEndInCanvas = end.x > canvas.left && end.y > canvas.top && end.x < canvas.right && end.y < canvas.bottom; //끝점이 캔버스 안에 있는지
        bool isDraggedInCanvas = isStartInCanvas && isEndInCanvas;
        if (isMouseInCanvas && isDraggedInCanvas) {
            switch (polygon_state) {
            case POLY_RECTANGLE:
                hbrush = CreateSolidBrush(RGB(171, 235, 52));
                SelectObject(hdc, hbrush);
                Rectangle(hdc, start.x, start.y, end.x, end.y);
                DeleteObject(hbrush);
                savePoly(tmpPoly, start.x, start.y, end.x, end.y, POLY_RECTANGLE);
                break;
            case POLY_ELLIPSE:
                hbrush = CreateSolidBrush(RGB(243, 166, 255));
                SelectObject(hdc, hbrush);
                Ellipse(hdc, start.x, start.y, end.x, end.y);
                DeleteObject(hbrush);
                savePoly(tmpPoly, start.x, start.y, end.x, end.y, POLY_ELLIPSE);
                break;
                /*case POLY_BONO:
                    center.x = (canvas.left + canvas.right) / 2;
                    center.y = (canvas.top + canvas.bottom) / 2;
                    h_radius = 100;
                    v_radius = 100;
                    DrawBonoBono(hdc, center.x, center.y, h_radius, v_radius, isSpaceBarPressed);
                    break;*/
            case POLY_RYAN:
                DrawRyan(hdc, center.x, center.y, start.y, end.y, h_radius, v_radius);
                break;
            case POLY_CUBE:
                hbrush = CreateSolidBrush(RGB(130, 94, 219));
                SelectObject(hdc, hbrush);
                DrawCube(hdc, start.x, start.y, end.x, end.y);
                DeleteObject(hbrush);
                savePoly(tmpPoly, start.x, start.y, end.x, end.y, POLY_CUBE);
                break;
            }
        }
        if (polygon_state == POLY_BONO && onBonobono) {
            center.x = (canvas.left + canvas.right) / 2;
            center.y = (canvas.top + canvas.bottom) / 2;
            h_radius = 100;
            v_radius = 100;
            DrawBonoBono(hdc, center.x, center.y, h_radius, v_radius, isSpaceBarPressed);
        }
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


