#include <iostream>
#include "./../src/Tree.hpp"
using namespace std;
using namespace DM852;
#include <assert.h>
#include <vector>


template <typename T, typename S>
class Entity {
private:
    Tree<T, S> t;
public:
    Entity(const Tree<T, S> &t) : t(t) {}

    Entity(Tree<T, S> &&t) : t((Tree<T, S>&&)t) {}

    ~Entity() {}

    void printName() {
        cout << "Hello world" << endl;
    }
};




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

    Tree<int, string> tree;
    for (int i = 0; i < 100; i++) {
        tree.insert(i, "123");
    }

    for (int i = 0; i < 50; i++) {
        tree.insert(i, "abc");
    }
    assert(tree.size() == 100);
    
    Tree<int, string>::iterator it = tree.begin();
    for (int i = 0; i < 100; i++) {
        if (i < 50) {
            assert(*it == "abc");
        } else {
            assert(*it == "123");
        }
        it++;
    }

    for (int i = 0; i < 25; i++) {
        tree.erase(i);
    }
    assert(tree.size() == 75);

    Tree<int, string>::iterator it2 = tree.begin();
    for (int i = 25; i < 100; i++) {
        if (i < 50) {
            assert(*it2 == "abc");
        } else {
            assert(*it2 == "123");
        }
        it2++;
    }

    for (int i = 0; i < 25; i++) {
        assert(tree.find(i) == tree.end());
    }
    cout << "\tsuccess" << endl;
}


/**
 * Tests whether the begin and end functions return the correct nodes.
 */
void test_begin_and_end() {
    cout << "Testing: test_begin_and_end" << endl;
    Tree<int, string> tree;
    assert(tree.begin() == tree.end());

    for (int i = 0; i < 100; i++) {
        tree.insert(i, to_string(i));
    }
    
    assert(*tree.begin() == "0");

    for (int i = 0; i < 10; i++) {
        tree.erase(i);
    }
    assert(*tree.begin() == "10");

    for (int i = 10; i < 100; i++) {
        tree.erase(i);
    }

    assert(tree.begin() == tree.end());
    cout << "\tsuccess" << endl;
}


/**
 * Tests whether the front and back functions return the correct values.
 */
void test_front_and_back() {
    cout << "Testing: test_front_and_back" << endl;
    Tree<int, string> tree;
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


    Tree<int, string> t1;
    t1.insert(10, "a");
    t1.insert(6, "b");
    t1.insert(2, "c");
    t1.insert(20, "d");
    t1.insert(15, "e");
    t1.insert(4, "f");
    t1.insert(5, "g");
    t1.insert(14, "h");
    t1.insert(16, "i");
    t1.insert(3, "j");
    Tree<int, string> t2(t1);

    assert(t1.front() == "c");
    assert(t1.back() == "d");

    t1.erase(2);
    assert(t1.front() == "j");
    assert(t1.back() == "d");

    t1.erase(14);
    assert(t1.front() == "j");
    assert(t1.back() == "d");

    t1.erase(20);
    assert(t1.front() == "j");
    assert(t1.back() == "i");

    t1.erase(10);
    assert(t1.front() == "j");
    assert(t1.back() == "i");

    t1.erase(3);
    assert(t1.front() == "f");
    assert(t1.back() == "i");

    t1.erase(5);
    assert(t1.front() == "f");
    assert(t1.back() == "i");

    t1.erase(4);
    assert(t1.front() == "b");
    assert(t1.back() == "i");

    t1.erase(16);
    assert(t1.front() == "b");
    assert(t1.back() == "e");

    t1.erase(15);
    assert(t1.front() == "b");
    assert(t1.back() == "b");

    t1.erase(6);
    assert(t1.begin() == t1.end());

    t2.clear();
    t2.insert(9, "a");
    assert(t2.front() == "a");
    assert(t2.front() == t2.back());

    cout << "\tsuccess" << endl;
}


/**
 * Test whether the clear function works properly.
 */
void test_clear() {
    cout << "Testing: test_clear" << endl;
    Tree<int, string> tree;
    for (int i = 0; i < 100; i++) {
        tree.insert(i, "Hello");
    }
    tree.clear();
    assert(tree.size() == 0);
    assert(tree.begin() == tree.end());


    for (int i = 0; i < 10000; i++) {
        tree.insert(rand(), "a");
    }
    tree.clear();
    assert(tree.size() == 0);
    assert(tree.begin() == tree.end());
    cout << "\tsuccess" << endl;
}


/**
 * Test equality comparisons between trees.
 */
void test_equality() {
    cout << "Testing: test_equality" << endl;
    Tree<int, string> tree1;
    Tree<int, string> tree2;
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
    Tree<int, string> tree1;
    Tree<int, string> tree2;
    for (int i = 0; i < 100; i++) {
        tree1.insert(i, "Hello");
    }
    assert(tree1 != tree2);

    tree2 = tree1;
    assert(tree2.size() == 100);
    assert(tree1 == tree2);

    tree1.erase(0);
    assert(tree1 != tree2);
    assert(tree2.size() == 100);

    Tree<int, string> tree3(tree2);
    assert(tree3 == tree2);
    assert(tree3.size() == 100);

    cout << "\tsuccess" << endl;
}


void test_inorder_iterator() {
    cout << "Testing: test_iterator" << endl;

    Tree<int, int> tree;
    tree.insert(1, 1);
    tree.insert(3, 3);
    tree.insert(2, 2);
    tree.insert(4, 4);
    for (int i = 0; i < 100; i++) {
        int x = rand();
        tree.insert(x, x);
    }
    
    int prev = 0;
    for (Tree<int, int>::iterator it = tree.begin(); it != tree.end(); ++it) {
        assert(prev < *it);
        prev = *it;
    }

    cout << "\tsuccess" << endl;
}


void test_movable() {
    cout << "Testing: test_movable" << endl;

    Tree<int, string> t;
    Entity<int, string> entity(std::move(t));

    Tree<vector<int>, vector<string>> vector_tree;
    vector_tree.insert(vector<int> {1, 2}, vector<string> {"hello", "world"});

    cout << "\tsuccess" << endl;
}


void test_comparison_type() {
    cout << "Testing: test_comparison_type" << endl;

    Tree<int, int, std::greater<int>> tree;
    for (int i = 0; i < 100; i++) {
        int x = rand();
        tree.insert(x, x);
    }
    int prev = RAND_MAX;
    for (Tree<int, int, std::greater<int>>::iterator it = tree.begin(); it != tree.end(); ++it) {
        assert(prev > *it);
        prev = *it;
    }

    cout << "\tsuccess" << endl;
}


int main() {
    test_insert_and_erase();
    test_begin_and_end();
    test_front_and_back();
    test_clear();
    test_equality();
    test_copyable();
    test_inorder_iterator();
    test_movable();
    test_comparison_type();
    return 0;
}