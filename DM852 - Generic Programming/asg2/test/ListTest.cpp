#include <iostream>
#include "./../src/List.hpp"
#include <assert.h>
#include <vector>

using namespace DM852;
using namespace std;

template <typename T>
class Entity {
private:
    List<T> m_Name;
public:
    Entity(const List<T> &name) : m_Name(name) {}

    // Takes in an temperary.
    Entity(List<T> &&name) : m_Name((List<T>&&)name) {}

    ~Entity() {}

    void printName() {
        cout << "Hello world" << endl;
    }
};



/**
 * Tests if the list deallocates itself and all its nodes.
 */
void test_basic_clear_up() {
    cout << "Testing: test_basic_clear_up" << endl;

    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    cout << "\tsuccess" << endl;
}

/**
 * Tests the invariant of size(). 
 */
void test_size() {
    cout << "Testing: test_size" << endl;

    List<string> list;
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

    List<string> list;

    list.push_back("First");
    List<string>::const_iterator first = list.begin();
    List<string>::const_iterator second = list.insert(first, "Second");

    assert(list.size() == 2);
    assert(*(list.begin()) == *second);

    //assert(list.back() == *first);
    List<string>::const_iterator third = list.insert(first, "Third");
    assert(list.size() == 3);
    assert(*(list.begin()) == *second);
    assert(list.back() == *first);

    list.erase(first);
    assert(list.size() == 2);

    list.erase(second);
    assert(*(list.begin()) == *third);
    cout << "\tsuccess" << endl;
}


/**
 * Tests if empty() is working properly in different scenarios.
 */
void test_empty() {
    cout << "Testing: test_empty" << endl;

    List<std::string> list;
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

    List<string>::iterator n = list.begin();
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

    List<std::string> list1;
    List<std::string> list2;

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

    List<std::string> list1;
    List<std::string> list2;

    list2 = list1;
    assert(list1 == list2);

    for (int i = 0; i < 10; i++) {
        list1.push_back("Test");
    }
    assert(list1 != list2);

    list2.push_back("test 2");
    assert(list1 != list2);

    list2 = list1;
    assert(list1 == list2);

    list1.pop_back();
    assert(list1 != list2);
    list2.pop_back();

    // Go forward through the list of nodes.
    for (List<string>::iterator n1 = list1.begin(), n2 = list2.begin(); (n1 != list1.end() && n2 != list2.end()); ++n1, ++n2) {
        assert(*n1 == *n2);
    }

    // Go backwards through the list of nodes.
    for (List<string>::iterator n1 = --(list1.end()), n2 = --(list2.end()); (n1 != list1.begin() && n2 != list2.begin()); --n1, --n2) {
        assert(*n1 == *n2);
    }

    // Copy construct list.
    List<std::string> list3(list2);

     // Go forward through the list of nodes.
    for (List<string>::iterator n3 = list3.begin(), n2 = list2.begin(); (n3 != list3.end() && n2 != list2.end()); ++n3, ++n2) {
        assert(*n3 == *n2);
    }

    // Go backwards through the list of nodes.
    for (List<string>::iterator n1 = --(list1.end()), n2 = --(list2.end()); (n1 != list1.begin() && n2 != list2.begin()); --n1, --n2) {
        assert(*n1 == *n2);
    }

    list3.insert(list3.begin(), "Beginning");

    cout << "\tsuccess" << endl;
}


void test_insert() {
    cout << "Testing: test_insert" << endl;
    
    List<string> l;
    l.insert(l.end(), "Hello");
    assert(l.front() == "Hello");
    assert(l.front() == l.back());
    l.clear();
    l.insert(l.begin(), "Hello2");
    assert(l.front() == "Hello2");
    assert(l.front() == l.back());

    cout << "\tsuccess" << endl;
}


void test_movable() {
    cout << "Testing: test_movable" << endl;
    
    Entity<string> entity(List<string> {"Hello", "World", "This is a test"});

    List<vector<int>> vector_list;
    vector_list.insert(vector_list.begin(), vector<int> {1, 2, 3, 4});

    cout << "\tsuccess" << endl;
}




int main() {
    test_basic_clear_up();
    test_size();
    test_insert_and_erase();
    test_empty();
    test_equality();
    test_copyable();
    test_insert();
    test_movable();
    return 0;
}

