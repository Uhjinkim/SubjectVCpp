#include <iostream>

// Node 구조체 정의
struct Node {
    int data;
    Node* next;

    // 생성자
    Node(int value) : data(value), next(nullptr) {}
};

// 스택 클래스 정의
class Queue {
private:
    Node* front; // 큐의 맨 앞 노드를 가리키는 포인터
    Node* rear;  // 큐의 맨 뒤 노드를 가리키는 포인터

public:
    // 생성자
    Queue() : front(nullptr), rear(nullptr) {}

    // 큐에 원소 추가
    void enqueue(int value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        std::cout << "Enqueued: " << value << std::endl;
    }

    // 스택에서 원소 제거 및 반환
    // 큐에서 원소 제거 및 반환
    int dequeue() {
        if (isEmpty()) {
            std::cerr << "큐가 비어있습니다." << std::endl;
            return -1; // 큐가 비어있을 경우 -1 반환
        }

        int dequeuedValue = front->data;
        Node* temp = front;
        front = front->next;
        delete temp;

        if (front == nullptr) {
            rear = nullptr;
        }

        std::cout << "Dequeued: " << dequeuedValue << std::endl;
        return dequeuedValue;
    }

    // 스택이 비어있는지 확인
    bool isEmpty() const {
        return front == nullptr;
    }

    // 큐의 맨 앞 원소 반환 (제거하지 않음)
    int peek() const {
        if (isEmpty()) {
            std::cerr << "큐가 비어있습니다." << std::endl;
            return -1; // 큐가 비어있을 경우 -1 반환
        }
        std::cout << "Peek: " << front->data << std::endl;
        return front->data;
    }
    void show() const {
        if (isEmpty()) {
            std::cout << "큐가 비어있습니다." << std::endl;
            return;
        }

        std::cout << "현재 큐 상태: \n";
        Node* current = front;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    // 큐 생성 및 테스트
    Queue myQueue;
    myQueue.show();
    myQueue.enqueue(1);
    myQueue.enqueue(2);
    myQueue.enqueue(3);
    myQueue.peek();
    myQueue.enqueue(4);
    myQueue.enqueue(5);

    return 0;
}