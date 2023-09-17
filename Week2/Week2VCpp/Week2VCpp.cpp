// Week2VCpp.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define ESC 27
#define CR 13
#define BKSPACE 8
#define MAX_LENGTH_OF_COMMAND 100


void clearArray(char arr[], int max) {
    for (int i = 0; i < max; i++) {
        arr[i] = 0;
    }

}

int main() {
    char input = 0;
    char command[MAX_LENGTH_OF_COMMAND] = "";
    int count;
    

    printf("Week 2 VC++\n\n");
    Sleep(500);
    printf("202007017 김우진\n\n");
    Sleep(500);
    printf("캐릭터: 이즈리얼\n");
    printf("스킬: q/w/e/r/d/f\n\n");
    Sleep(500);
    while(1) {
        printf("command>");
        count = -1;
        //_getch()로 입력받고 화면에 출력, 엔터 누를 때까지 반복
        for (int i = 0; i < MAX_LENGTH_OF_COMMAND+1; i++) {
            input = _getch();
            printf("%c", input);

            if (input == BKSPACE) { //백스페이스 입력 시 입력된 문자 제거(빠르게 연타하거나 길게 누르면 안될 수도 있음)
                if (count >= 0) {
                    printf(" ");
                    printf("\b");
                    command[i] = 0;
                    i -= 2; // 지운 자리에 새로 입력할 수 있도록 배열 자릿수 앞당김
                }
                else { // "command>" 부분이 지워지지 않도록 배열 자릿수가 0보다 작아지면 '>' 위치에 '>' 를 출력하여 침범 방지
                    printf(">");
                    i--;
                }

            }
            else if(input == ESC) {
                printf(">종료: 프로그램을 종료합니다.");
                return 0;
            }
            else if (input == CR) {
                printf("\n");
                command[i] = 0; //엔터 키 입력 시 배열 끝에 \0 삽입
                count = i;
                break;
            }
            else if (i >= 0) {
                //특정 키 이외 나머지 조건: 입력되는 문자를 배열에 저장
                if (i >= MAX_LENGTH_OF_COMMAND) {
                    printf("\n글자 수 초과, 다시 입력하세요.\ncommand>");
                    i = -1;
                    //배열 자릿수 초과 시 재입력 요구하고 다시 0번째 자리로 이동
                }
                else {
                    command[i] = input;
                }
                }
            count = i;
        }
        boolean isEsterEgg = command[0] == 'c' && command[1] == 'o' && command[2] == 'm' && command[3] == 'm' && command[4] == 'a' && command[5] == 'n' && command[6] == 'd' && command[7] == 0;
        if (command[1] == 0) { //command 배열 2번째 자리 널문자를 체크해서 명령어 감지
            if (command[0] == 'q') {
                printf("q: 신비한 화살\n");
            }
            else if (command[0] == 'w') {
                printf("w: 정수의 흐름\n");
            }
            else if (command[0] == 'e') {
                printf("e: 비전 이동\n");
            }
            else if (command[0] == 'r') {
                printf("r: 정조준 일격\n");
            }
            else if (command[0] == 'd') {
                printf("스펠1: 회복\n");
            }
            else if (command[0] == 'f') {
                printf("스펠2: 점멸\n");
            }
            else {
                printf("잘못된 입력...\n");
            }
        }
        else if (isEsterEgg) {
            printf("와! 이스터에그!\n");
        }
        else {
            printf("잘못된 입력...\n");
        }
        clearArray(command, count); //command 배열 초기화
    }
    return 202007017;
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
