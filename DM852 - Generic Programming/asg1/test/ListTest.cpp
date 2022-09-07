#include <iostream>
#include "./../src/List.hpp"
#include <assert.h>

using namespace DM852;
using namespace std;

/**
 * Tests if the list deallocates itself and all its nodes.
 */
void test_basic_clear_up() {
    cout << "Testing: test_basic_clear_up" << endl;

    List list;
    list.push_back("Test");
    list.push_back("Test");
    list.push_back("Test");

    cout << "\tsuccess" << endl;
}

/**
 * Tests the invariant of size(). 
 */
void test_size() {
    cout << "Testing: test_size" << endl;

    List list;
    assert(list.size() == 0);

    list.push_back("1");
    assert(list.size() == 1);
    
    list.push_back("2");
    assert(list.size() == 2);
    
    list.push_back("3");
    assert(list.size() == 3);

    list.pop_back();
    assert(list.size() == 2);

    list.push_back("3");
    assert(list.size() == 3);

    list.pop_back();
    assert(list.size() == 2);

    list.pop_back();
    assert(list.size() == 1);

    list.pop_back();
    assert(list.size() == 0);

    for (int i = 0; i < 100; i++) {
        list.push_back("abc");
    }
    assert(list.size() == 100);

    list.clear();
    assert(list.size() == 0);
    
    cout << "\tsuccess" << endl;
}


/**
 * Tests if insert and erase works correctly.
 */
void test_insert_and_erase() {
    cout << "Testing: test_insert_and_erase" << endl;

    List list;
    assert(list.begin() == nullptr);

    list.push_back("First");
    LIST_NODE::Node *first = list.begin();
    LIST_NODE::Node *second = list.insert(first, "Second");
    assert(list.size() == 2);
    assert(list.begin() == second);
    assert(list.back() == first->data);

    LIST_NODE::Node *third = list.insert(first, "Third");
    assert(list.size() == 3);
    assert(list.begin() == second);
    assert(list.back() == first->data);
    assert(second->next == third);
    assert(first->prev == third);

    list.erase(first);
    assert(list.size() == 2);
    assert(list.back() == third->data);
    assert(third->next == nullptr);

    list.erase(second);
    assert(list.begin() == third);
    assert(third->prev == nullptr);
    assert(third->next == nullptr);

    cout << "\tsuccess" << endl;
}


/**
 * Tests if empty() is working properly in different scenarios.
 */
void test_empty() {
    cout << "Testing: test_empty" << endl;

    List list;
    assert(list.empty());

    list.push_back("Test");
    assert(!list.empty());

    list.pop_back();
    assert(list.empty());

    for (int i = 0; i < 100; i++) {
        list.push_back("Test");
    }    
    assert(!list.empty());

    list.clear();
    assert(list.empty());

    list.push_back("Test");

    LIST_NODE::Node *n = list.begin();
    list.insert(n, "Test2");
    assert(!list.empty());

    list.erase(n);
    assert(!list.empty());

    list.erase(list.begin());
    assert(list.empty());

    cout << "\tsuccess" << endl;
}


void test_equality() {
    cout << "Testing: test_equality" << endl;

    List list1;
    List list2;

    assert(list1 == list2);

    list1.push_back("1");
    assert(list1 != list2);

    list2.push_back("2");
    assert(list1 != list2);

    list2.clear();
    list2.push_back("1");
    assert(list1 == list2);

    list2.push_back("2");
    assert(list1 != list2);

    cout << "\tsuccess" << endl;
}


void test_copyable() {
    cout << "Testing: test_copyable" << endl;

    List list1;
    List list2;

    list2 = list1;
    assert(list1 == list2);

    for (int i = 0; i < 10; i++) {
        list1.push_back("Test");
    }
    assert(list1 != list2);

    list2 = list1;    
    assert(list1 == list2);
    
    list1.pop_back();
    assert(list1 != list2);
    list2.pop_back();

    // Go forward through the list of nodes.
    LIST_NODE::Node *node1 = list1.begin();
    LIST_NODE::Node *node2 = list2.begin();
    while (node1->next != nullptr) {
        assert(node1->data == node2->data);
        node1 = node1->next;
        node2 = node2->next;
    }

    // Go backwards through the list of nodes.
    while (node1 != nullptr) {
        assert(node1->data == node2->data);
        node1 = node1->prev;
        node2 = node2->prev;
    }

    // Copy construct list.
    List list3(list2);

     // Go forward through the list of nodes.
    LIST_NODE::Node *node3 = list3.begin();
    node2 = list2.begin();
    while (node3->next != nullptr) {
        assert(node3->data == node2->data);
        node3 = node3->next;
        node2 = node2->next;
    }

    // Go backwards through the list of nodes.
    while (node3 != nullptr) {
        assert(node3->data == node2->data);
        node3 = node3->prev;
        node2 = node2->prev;
    }


    cout << "\tsuccess" << endl;
}

void test_const() {
    cout << "Testing: test_const" << endl;

    List list1;
    for (int i = 0; i < 100; i++) {
        list1.push_back(to_string(i));
    }

    const List list2 = list1;
    assert(list2.front() == "0");
    assert(list1.back() == "99");

    const LIST_NODE::Node *n1 = list2.begin();
    assert(n1->data == "0");

    const LIST_NODE::Node *n2 = list2.end();
    assert(n2 == nullptr);

    cout << "\tsuccess" << endl;
}

int main() {
    test_basic_clear_up();
    test_size();
    test_insert_and_erase();
    test_empty();
    test_equality();
    test_copyable();
    test_const();
    return 0;
}