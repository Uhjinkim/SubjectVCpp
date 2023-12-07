#include "Resource.h"
#include "framework.h"
#include "Bonobono.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
RECT rcClient = { 0, 0, VIEW_WIDTH, VIEW_HEIGHT }; //Ŭ���̾�Ʈ ���� ���� ���� �簢��
RECT client, layout, canvas{}; // ū �ڽ��� ���̾ƿ�, ����׿����� ĵ������� ����
HWND hbuttons[BUTTON_COUNT]; //�ݺ��� ����� ��ư ����
int polygon_state = POLY_RECTANGLE; // ���� �׸��� ������ ��� ����: �ʱⰪ �簢��

//���̾ƿ��� ĵ���� ����
const int margin = 8;
const int padding = 8;

bool isMouseInCanvas; //���� ���콺 Ŀ���� ����� ���� �ȿ� �ִ���
bool onBonobono = false;

//�ݺ��� ����Ͽ� ��ư 5���� �����ϱ� ���� ����ü
struct BUTTON {
    WCHAR name[BUTTON_COUNT][10] = { L"���簢��", L"Ÿ��", L"���뺸��", L"���̾�", L"ť��" };
    int width = DFBUTTON_WIDTH;
    int height = DFBUTTON_HEIGHT;
    int margin = 16; //��ư�� ����
    int menu[BUTTON_COUNT] = { POLY_RECTANGLE, POLY_ELLIPSE, POLY_BONO, POLY_RYAN, POLY_CUBE };
};

BUTTON btn{};
POLY tmpPoly{};
POINT start, end, mPos, axis, center, grab{};
BOOL isDrawing = FALSE; //���� �׸��� �ִ���
BOOL isEditing = FALSE; //ũ�������̳� �̵��ϰ� �ִ���
BOOL isSpaceBarPressed = FALSE;

double dx, dy;
double angle;
int pWidth, pHeight;

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BONOBONO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }


    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BONOBONO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}




//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
        // �޴� ������ ���� �м��մϴ�:
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
            angle = static_cast<double>(pHeight) / pWidth; // ���� ���� ���� ���� ���� ����
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
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        GetClientRect(hWnd, &client);
        hbrush = CreateSolidBrush(RGB(255, 240, 200));
        SelectObject(hdc, hbrush);
        FillRect(hdc, &client, hbrush); //��� ä���
        //��ư ũ��. ��ġ ����
        btn.width = (layout.right - layout.left) / BUTTON_COUNT - btn.margin * 2;
        for (int i = 0; i < BUTTON_COUNT; i++) {
            SetWindowPos(hbuttons[i], NULL, layout.left + (layout.right - layout.left) / BUTTON_COUNT * (i)+btn.margin, layout.top + btn.margin, btn.width, btn.height, SWP_NOZORDER);
        }

        //�����̳� �ڽ�, ����׿��� �׸���
        Rectangle(hdc, layout.left, layout.top, layout.right, layout.bottom);
        Rectangle(hdc, canvas.left, canvas.top, canvas.right, canvas.bottom);
        DeleteObject(hbrush);

        //�����̽��� ���ȴ��� üũ
        if (GetKeyState(VK_SPACE) < 0) {
            isSpaceBarPressed = TRUE;
        }
        else {
            isSpaceBarPressed = FALSE;
        }

        //�巡�� �������� ������ �߽�
        center.x = (start.x + end.x) / 2;
        center.y = (start.y + end.y) / 2;

        h_radius = start.x < end.x ? (end.x - start.x) / 2 : (start.x - end.x) / 2; //���� ������ ��
        v_radius = start.y < end.y ? (end.y - start.y) / 2 : (start.y - end.y) / 2; //���� ������ ��
        bool isStartInCanvas = start.x > canvas.left && start.y > canvas.top && start.x < canvas.right && start.y < canvas.bottom; //�������� ĵ���� �ȿ� �ִ���
        bool isEndInCanvas = end.x > canvas.left && end.y > canvas.top && end.x < canvas.right && end.y < canvas.bottom; //������ ĵ���� �ȿ� �ִ���
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
            DrawBonobono(hdc, center.x, center.y, h_radius, v_radius, isSpaceBarPressed);
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