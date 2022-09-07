#include <iostream>
#include "./../src/Tree.hpp"
using namespace std;
using namespace DM852;
#include <assert.h>

/**
 * Test the insert and erase function if they work correctly.
 * Insert should be able to insert new nodes in the tree and update
 * current ones with the same key.
 * Erase should be able to delete nodes with the given key and if a
 * node with the given key doesn't exists nothing happens.
 * It also tests find, if the nodes inserted can be found or not.
 */
void test_insert_and_erase() {
    cout << "Testing: test_insert_and_erase" << endl;
    Tree tree;
    for (int i = 0; i < 100; i++) {
        tree.insert(i, "123");
    }

    for (int i = 0; i < 50; i++) {
        tree.insert(i, "abc");
    }
    assert(tree.size() == 100);
    for (int i = 0; i < 100; i++) {
        if (i < 50) {
            assert(tree.find(i)->value == "abc");
        } else {
            assert(tree.find(i)->value == "123");
        }
    }

    for (int i = 0; i < 25; i++) {
        tree.erase(i);
    }

    assert(tree.size() == 75);
    for (int i = 25; i < 100; i++) {
        if (i < 50) {
            assert(tree.find(i)->value == "abc");
        } else {
            assert(tree.find(i)->value == "123");
        }
    }

    for (int i = 0; i < 25; i++) {
        assert(tree.find(i) == nullptr);
    }

    cout << "\tsuccess" << endl;
}


/**
 * Tests whether the begin and end functions return the correct nodes.
 */
void test_begin_and_end() {
    cout << "Testing: test_begin_and_end" << endl;

    Tree tree;
    assert(tree.begin() == nullptr);

    for (int i = 0; i < 100; i++) {
        tree.insert(i, to_string(i));
    }
    assert(tree.begin()->key == 0);
    assert(tree.end() == nullptr);

    for (int i = 0; i < 10; i++) {
        tree.erase(i);
    }

    assert(tree.begin()->key == 10);
    assert(tree.end() == nullptr);

    for (int i = 10; i < 100; i++) {
        tree.erase(i);
    }

    assert(tree.begin() == nullptr);
    assert(tree.end() == nullptr);
        
    cout << "\tsuccess" << endl;
}


/**
 * Tests whether the front and back functions return the correct values.
 */
void test_front_and_back() {
    cout << "Testing: test_clear" << endl;

    Tree tree;
    tree.insert(100, "1");

    assert(tree.front() == "1");
    assert(tree.back() == "1");

    tree.insert(200, "a");
    assert(tree.front() == "1");
    assert(tree.back() == "a");

    tree.insert(50, "2");
    assert(tree.front() == "2");
    assert(tree.back() == "a");

    tree.erase(100);
    assert(tree.front() == "2");
    assert(tree.back() == "a");
    
    tree.erase(50);
    assert(tree.front() == "a");
    assert(tree.back() == "a");

    tree.clear();
    int max = 0;
    int min = RAND_MAX;
    for (int i = 0; i < 10000; i++) {
        int value = rand();
        if (value > max)
            max = value;
        if (value < min)
            min = value;
        tree.insert(value, to_string(value));
    }

    assert(tree.front() == to_string(min));
    assert(tree.back() == to_string(max));

    cout << "\tsuccess" << endl;
}


/**
 * Test whether the clear function works properly.
 */
void test_clear() {
    cout << "Testing: test_clear" << endl;

    Tree tree;
    for (int i = 0; i < 100; i++) {
        tree.insert(i, "Hello");
    }
    tree.clear();
    assert(tree.size() == 0);
    assert(tree.begin() == nullptr);


    for (int i = 0; i < 10000; i++) {
        tree.insert(rand(), "a");
    }
    tree.clear();
    assert(tree.size() == 0);
    assert(tree.begin() == nullptr);

    cout << "\tsuccess" << endl;
}


/**
 * Test equality comparisons between trees.
 */
void test_equality() {
    cout << "Testing: test_equality" << endl;

    Tree tree1;
    Tree tree2;
    for (int i = 0; i < 100; i++) {
        tree1.insert(i, "Hello");
        tree2.insert(i, "Hello");
    }
    assert(tree1 == tree2);

    tree2.insert(99, "Hello World");
    assert(tree1 != tree2);

    tree2.erase(99);
    assert(tree1 != tree2);

    tree1.clear();
    tree2.clear();
    assert(tree1 == tree2);

    cout << "\tsuccess" << endl;
}

/**
 * Tests assignment from one tree to another. 
 */
void test_copyable() {
    cout << "Testing: test_copyable" << endl;

    Tree tree1;
    Tree tree2;
    for (int i = 0; i < 100; i++) {
        tree1.insert(i, "Hello");
    }
    assert(tree1 != tree2);

    tree2 = tree1;
    assert(tree1 == tree2);

    tree1.erase(0);
    assert(tree1 != tree2);

    Tree tree3(tree2);
    assert(tree3 == tree2);

    cout << "\tsuccess" << endl;
}


void test_node_nullptr() {
    cout << "Testing: test_node_nullptr" << endl;

    Tree tree;
    tree.insert(100, "Hello World!");
    assert(tree.begin()->next() == nullptr);
    assert(tree.begin()->prev() == nullptr);

    cout << "\tsuccess" << endl;
}


void test_const() {
    cout << "Testing: test_const" << endl;
    Tree tree;

    for (int i = 0; i < 100; i++) {
        tree.insert(i, to_string(i));
    }
    const Tree tree2 = tree;

    assert(tree2.front() == "0");
    assert(tree2.back() == "99");

    const TREE_NODE::Node *n = tree2.begin();
    assert(n->key == 0);
    assert(n->value == "0");

    const TREE_NODE::Node *n2 = tree2.end();
    assert(n2 == nullptr);

    cout << "\tsuccess" << endl;
}


int main() {
    test_insert_and_erase();
    test_begin_and_end();
    test_front_and_back();
    test_clear();
    test_equality();
    test_copyable();
    test_node_nullptr();
    test_const();
    return 0;
}