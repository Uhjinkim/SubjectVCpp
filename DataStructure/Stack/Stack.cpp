#include <iostream>

// Node ����ü ����
struct Node {
    int data;
    Node* next;

    // ������
    Node(int value) : data(value), next(nullptr) {}
};

// ���� Ŭ���� ����
class Stack {
private:
    Node* top; // ������ �� �� ��带 ����Ű�� ������

public:
    // ������
    Stack() : top(nullptr) {}

    // ���ÿ� ���� �߰�
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        std::cout << "Pushed: " << value << std::endl;
    }

    // ���ÿ��� ���� ���� �� ��ȯ
    int pop() {
        if (isEmpty()) {
            std::cerr << "������ ����ֽ��ϴ�." << std::endl;
            return -1; // ������ ������� ��� -1 ��ȯ
        }

        int poppedValue = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        std::cout << "Popped: " << poppedValue << std::endl;
        return poppedValue;
    }

    // ������ ����ִ��� Ȯ��
    bool isEmpty() const {
        return top == nullptr;
    }

    // ������ �� �� ���� ��ȯ (�������� ����)
    int peek() const {
        if (isEmpty()) {
            std::cerr << "������ ����ֽ��ϴ�." << std::endl;
            return -1; // ������ ������� ��� -1 ��ȯ
        }
        std::cout << "Peek: " << top->data << std::endl;
        return top->data;
    }
    void show() const {
        if (isEmpty()) {
            std::cout << "������ ����ֽ��ϴ�." << std::endl;
            return;
        }

        std::cout << "���� ���� ����: \n";
        Node* current = top;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    // ���� ���� �� �׽�Ʈ
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