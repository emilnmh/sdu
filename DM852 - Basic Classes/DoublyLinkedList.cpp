#include <iostream>

class Node {
    int value = 0;
    Node *predecessor = nullptr, *successor = nullptr;
public:
    Node(int value) : value(value) { }
    
    ~Node() { }

    /**
        * Sets the value.
        */
    void setValue(int value) {
        this->value = value;
    }

    /**
        * Sets the successor of the node.
        */
    void setSuccessor(Node *successor) {
        this->successor = successor;
    }

    /**
        * Sets the predecessor of the node.
        */
    void setPredecessor(Node *predecessor) {
        this->predecessor = predecessor;
    }

    /**
        * Returns the value of the node.
        */
    int getValue() {
        return value;
    }

    /**
        * Returns the successor of the node.
        */
    Node *getSuccessor() {
        return successor;
    }

    /**
        * Returns the predecessor of the node.
        */
    Node *getPredecessor() {
        return predecessor;
    }
};

class DoublyLinkedList {
    Node *head = nullptr; // The first node in the doubly linked list.
    Node *tail = nullptr; // The last node in the doubly linked list.
    int length = 0; // The length of the doubly linked list.
public:
    DoublyLinkedList() = default;
    ~DoublyLinkedList() { 
        Node *current = head;
        Node *next;
        while (current != nullptr) {
            next = current->getSuccessor();
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
    }


    int operator[](int i) {
        int value;
        Node *current = head;
        while (i != 0 && current != nullptr) {
            current = current->getSuccessor();
            i--;
        }
        return current->getValue();
    }

    /**
     * Pushes a new value to the list.
     */
    void push(int value) {
        Node *newNode = new Node(value);
        newNode->setPredecessor(tail);
        if (head == nullptr) {
            head = newNode;
        } else {
            tail->setSuccessor(newNode);
        }
        tail = newNode;
        length++;
    }

    /**
     * Deletes and returns the last element in the list.
     * Precondition: getLength() > 0.
     */
    int pop() {
        int value;
        if (tail != nullptr) {
            value = tail->getValue();
            if (tail == head) {
                delete tail;
                head = nullptr;
                tail = nullptr;
            } else {
                Node *newTail = tail->getPredecessor();
                delete tail;
                tail = newTail;
                tail->setSuccessor(nullptr);
            }

            length--;
        }
        return value;
    }


    int getLength() {
        return length;
    }
};

int main() {
    DoublyLinkedList dll;
    dll.push(1);
    dll.push(2);
    dll.push(3);    
    dll.pop();
    dll.pop();
    dll.pop();
    dll.pop();
    dll.push(1234);
    dll.pop();
    std::cout << dll.pop() << std::endl;
    
    return 0;
}