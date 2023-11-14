#include <iostream>

// Node 구조체 정의
struct Node {
    int data;
    Node* next;

    // 생성자
    Node(int value) : data(value), next(nullptr) {}
};

// 스택 클래스 정의
class Stack {
private:
    Node* top; // 스택의 맨 위 노드를 가리키는 포인터

public:
    // 생성자
    Stack() : top(nullptr) {}

    // 스택에 원소 추가
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        std::cout << "Pushed: " << value << std::endl;
    }

    // 스택에서 원소 제거 및 반환
    int pop() {
        if (isEmpty()) {
            std::cerr << "스택이 비어있습니다." << std::endl;
            return -1; // 스택이 비어있을 경우 -1 반환
        }

        int poppedValue = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        std::cout << "Popped: " << poppedValue << std::endl;
        return poppedValue;
    }

    // 스택이 비어있는지 확인
    bool isEmpty() const {
        return top == nullptr;
    }

    // 스택의 맨 위 원소 반환 (제거하지 않음)
    int peek() const {
        if (isEmpty()) {
            std::cerr << "스택이 비어있습니다." << std::endl;
            return -1; // 스택이 비어있을 경우 -1 반환
        }
        std::cout << "Peek: " << top->data << std::endl;
        return top->data;
    }
    void show() const {
        if (isEmpty()) {
            std::cout << "스택이 비어있습니다." << std::endl;
            return;
        }

        std::cout << "현재 스택 상태: \n";
        Node* current = top;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    // 스택 생성 및 테스트
    Stack myStack;
    myStack.show();
    myStack.push(1);
    myStack.push(2);
    myStack.push(3);
    myStack.peek();
    myStack.show();
    myStack.peek();
    myStack.pop();
    myStack.pop();
    myStack.show();
    myStack.peek();

    return 0;
}