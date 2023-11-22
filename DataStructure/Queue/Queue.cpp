#include <iostream>

// Node ����ü ����
struct Node {
    int data;
    Node* next;

    // ������
    Node(int value) : data(value), next(nullptr) {}
};

// ���� Ŭ���� ����
class Queue {
private:
    Node* front; // ť�� �� �� ��带 ����Ű�� ������
    Node* rear;  // ť�� �� �� ��带 ����Ű�� ������

public:
    // ������
    Queue() : front(nullptr), rear(nullptr) {}

    // ť�� ���� �߰�
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

    // ���ÿ��� ���� ���� �� ��ȯ
    // ť���� ���� ���� �� ��ȯ
    int dequeue() {
        if (isEmpty()) {
            std::cerr << "ť�� ����ֽ��ϴ�." << std::endl;
            return -1; // ť�� ������� ��� -1 ��ȯ
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

    // ������ ����ִ��� Ȯ��
    bool isEmpty() const {
        return front == nullptr;
    }

    // ť�� �� �� ���� ��ȯ (�������� ����)
    int peek() const {
        if (isEmpty()) {
            std::cerr << "ť�� ����ֽ��ϴ�." << std::endl;
            return -1; // ť�� ������� ��� -1 ��ȯ
        }
        std::cout << "Peek: " << front->data << std::endl;
        return front->data;
    }
    void show() const {
        if (isEmpty()) {
            std::cout << "ť�� ����ֽ��ϴ�." << std::endl;
            return;
        }

        std::cout << "���� ť ����: \n";
        Node* current = front;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    // ť ���� �� �׽�Ʈ
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