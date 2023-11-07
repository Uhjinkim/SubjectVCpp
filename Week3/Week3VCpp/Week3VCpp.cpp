// Week3VCpp.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;

#define COMBO_VAILD_MS 2000
#define MAX_LENGTH_OF_COMMAND 5
#define ASCII_ARROWKEY -32
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
#define ESC 27

//global var
bool isTimeOut;


void clearArr(char arr[], int max) {
    for (int i = 0; i < max; i++) {
        arr[i] = 0;
    }
}

int inspectArr(char arr[], int max, int arrow_arr[]) {
    for (int i = 0; i < max; i++) {
        if (i == arrow_arr[i]) {
            switch (arr[i]) {
            case LEFT_ARROW:
                printf("←");
                break;
            case RIGHT_ARROW:
                printf("→");
                break;
            case UP_ARROW:
                printf("↑");
                break;
            case DOWN_ARROW:
                printf("↓");
                break;
            default:
                printf("%c", arr[i]);
                break;
            }
        }
    }
    return 1;
}

void timer() {
    cout << "타이머 시작" << endl;
    std::this_thread::sleep_for(milliseconds(COMBO_VAILD_MS));
    isTimeOut = true;
    if (isTimeOut) {
        cout << "time over" << endl;
        isTimeOut = false;
    }
}

int main()
{
    char input;
    char combo[MAX_LENGTH_OF_COMMAND] = "";
    int count = 0;
    for (count = 0; count < MAX_LENGTH_OF_COMMAND; count++) {
        input = _getch();
        if (input == -32) {
            input = _getch();
            switch (input) {
            case LEFT_ARROW:
                printf("←");
                break;
            case RIGHT_ARROW:
                printf("→");
                break;
            case UP_ARROW:
                printf("↑");
                break;
            case DOWN_ARROW:
                printf("↓");
                break;
            default:
                break;
            }
        }
        if (input == 27) {
            break;
        }
        if (count == 0) {
            thread t1(timer);
            t1.detach();
        }
        std::cout << int(input);
        count = -1;
    }
}



// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
