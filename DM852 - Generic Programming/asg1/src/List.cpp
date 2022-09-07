#include "List.hpp"
#include <iostream>
#include <assert.h>
using namespace DM852;
using namespace LIST_NODE;

/**
 * Construct a new List object.
 */
List::List() { }


List::List(const List &other) {
    const Node *current = other.begin();

    // Copy all the nodes.
    while (current != nullptr) {
        Node *node = new Node();

        // If the head isn't set.
        if (this->head == nullptr)
            this->head = node;

        // Copy the data, and redirect pointers.
        node->data = current->data;
        if (current->prev != nullptr) {
            node->prev = this->tail;
            this->tail->next = node;
        }
        this->tail = node;

        current = current->next;
    }

    this->length = other.size();
}


/**
 * Destroy the List object.
 */
List::~List() {
    Node *current = this->head;
    Node *next;

    // Deallocates all the nodes.
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
}

/**
 * Returns the number of elements stored in the list.
 * 
 * @return int Number of elements in the list.
 */
int List::size() const {
    return this->length;
}


/**
 * Returns true iff the list is empty.
 * 
 * @return true | false
 */
bool List::empty() const {
    return this->length == 0;
}


/**
 * Inserts a new element in the end.
 * 
 * @param elem The new element to be inserted.
 */ 
void List::push_back(const std::string &elem) {
    // Creating a new node.
    Node *n = new Node();
    n->data = elem;

    if (empty()) { // If the list is empty.
        this->head = n;
        this->tail = n;
    } else { // Otherwise.
        this->tail->next = n;
        n->prev = this->tail;
        this->tail = n;
    }

    // Increment size.
    this->length++;
}


/**
 * Inserts a new element before the one stored in the node.
 * @pre node != nullptr and node is in the list.
 * 
 * @param node The node that new node should be inserted infront of.
 * @param elem The value of the new node.
 * 
 * @return The inserted node.
 */ 
Node *List::insert(Node *node, const std::string &elem) {
    // Creating a new node.
    Node *n = new Node();
    n->data = elem;

    // Correct pointers.
    if (node->prev != nullptr) {
         node->prev->next = n;
    } else { // New head.
        this->head = n;
    }
    n->prev = node->prev;
    node->prev = n;
    n->next = node;

    // Increment size.
    this->length++;

    return n;
}


/**
 * Erase all elements.
 */ 
void List::clear() {
    Node *current = this->head; 
    Node *next;
    // Iterate over list and delete elements.
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }

    // Reset fields.
    this->length = 0;
    this->head = nullptr;
    this->tail = nullptr;
}


/**
 * Erase the last element.
 */
void List::pop_back() {
    // Set new tail.
    Node *oldTail = this->tail;

    if (oldTail != nullptr) {
        this->tail = oldTail->prev;
    
        // If it is the last element.
        if (oldTail->prev == nullptr) this->head = nullptr;
        else this->tail->next = nullptr;

        delete oldTail;

        // Decrement size.
        this->length--;
    }
}


/**
 * Erase the element.
 * @pre *node != nullptr and node is in the list.
 * 
 * @param node The node to be deleted.
 */ 
void List::erase(Node *node) {
    // Correct pointers.
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        this->head = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        this->tail = node->prev;
    }

    // Delete node
    delete node;

    // Decremenet size
    this->length--;
}


/**
 * The first element.
 * 
 * @return First element in the list.
 */
std::string &List::front() {
    return this->head->data;
}


/**
 * The first element.
 * 
 * @return Const first element in the list.
 */
const std::string &List::front() const {
    return this->head->data;
}


/**
 * The last element.
 * 
 * @return Last element in the list. 
 */
std::string &List::back() {
    return this->tail->data;
}


/**
 * The last element.
 * 
 * @return Const last element in the list.
 */
const std::string &List::back() const {
    return this->tail->data;
}


/**
 * The first node.
 * 
 * @return Node* the first node in the list. 
 */
Node *List::begin() {
    return this->head;
}


/**
 * The first node.
 * 
 * @return Const Node* the first node in the list. 
 */
const Node *List::begin() const {
    return this->head;
}


/**
 * Nullptr.
 * 
 * @return Node* nullptr.
 */
Node *List::end() {
    return nullptr;
}

/**
 * Nullptr.
 * 
 * @return Const Node* nullptr
 */
const Node *List::end() const {
    return nullptr;
}


/**
 * Makes a copy of the other list.
 * 
 * @param other The other list that a copy should be made of.
 * 
 * @return List& The copy of the other list.
 */
List &List::operator=(const List &other) {
    const Node *current = other.begin();

    // Copy all the nodes.
    while (current != nullptr) {
        Node *node = new Node();

        // If the head isn't set.
        if (this->head == nullptr)
            this->head = node;

        // Copy the data, and redirect pointers.
        node->data = current->data;
        if (current->prev != nullptr) {
            node->prev = this->tail;
            this->tail->next = node;
        }
        this->tail = node;

        current = current->next;
    }

    this->length = other.size();

    return *this;
}


/**
 * Compares equality with other list.
 * 
 * @param other The other list that the list should be compared to.
 * @return true if the two list are equal.
 */
bool List::operator==(const List &other) const {
    // Check if the length of both list are equal.
    // Otherwise they can't be equal.
    if (this->size() == other.size()) {
        const Node *thisCurrent = this->begin();
        const Node *otherCurrent = other.begin();

        // Go through all the elements to check if their data is identical.
        while (thisCurrent != nullptr && thisCurrent->data == otherCurrent->data) {
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        } 

        return thisCurrent == nullptr;
    }
    return false;
}


/**
 * Compares inequality with other list. 
 * 
 * @param other The other list that the list should be compared to.
 * @return true if the two list are not equal.
 */
bool List::operator!=(const List &other) const {
    return !this->operator==(other);
}