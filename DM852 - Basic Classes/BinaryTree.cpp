#include <iostream>


class Node {
    int key = 0, value = 0;
    Node *left = nullptr, *right = nullptr, *parent;
public:
    Node(int key, int value) : key(key), value(value) { }

    ~Node() { }

    /**
     * Sets the left child.
     */
    void setLeft(Node *left) {
        this->left = left;
    }

    /**
     * Sets the right child.
     */
    void setRight(Node *right) {
        this->right = right;
    }

    /**
     * Setst the parent.
     */
    void setParent(Node *parent) {
        this->parent = parent;
    }

    /**
     * Returns the left child of the node.
     */
    Node *getLeft() {
        return this->left;
    }

    /**
     * Returns the right child of the node.
     */
    Node *getRight()  {
        return this->right;
    }

    /**
     * Returns the parent of the node.
     */
    Node *getParent() {
        return this->parent;
    }

    /**
     * Returns the key of the node.
     */
    int getKey() {
        return key;
    }

    /**
     * Returns the value of the node.
     */
    int getValue() {
        return value;
    }
};

class BinaryTree {
    Node *root = nullptr;
public:
    /**
     * Inserts new node into the tree.
     */
    void insertNode(int key, int value) {
        Node *current = nullptr;
        Node *next = root;

        // Find position for insert.        
        while (next != nullptr) {
            current = next;
            if (key < next->getKey()) {
                next = next->getLeft();
            } else {
                next = next->getRight();
            }
        }

        // Insert node.
        Node *newNode = new Node(key, value);
        newNode->setParent(current);

        if (current == nullptr) {
            this->root = newNode;
        } else if (key <= current->getKey()) {
            current->setLeft(newNode);
        } else {
            current->setRight(newNode);
        }
    }

    /**
     * Shifts the subtree, between the parent and child.
     */
    void shift(Node *parent, Node *child) {
        if (parent->getParent() == nullptr) {
            std::cout << 1 << std::endl;
            this->root = child;
        } else if (parent == parent->getParent()->getLeft()) {
            std::cout << 2 << std::endl;
            parent->getParent()->setLeft(child);
        } else {
            std::cout << 3 << std::endl;
            parent->getParent()->setRight(child);
        }

        if (child != nullptr) {
            child->setParent(parent->getParent());
        }
    } 


    /**
     * Deletes the first node with the given key.
     * Precondition: Node with key exists.
     */ 
    void deleteNode(int key) {
        Node *node = findNode(key);
        if (node->getLeft() == nullptr) {
            shift(node, node->getRight());
        } else if (node->getRight() == nullptr) {
            shift(node, node->getLeft());
        } else {
            
            Node *successor = this->successor(node);
            if (successor->getParent() != node) {
                shift(successor, successor->getRight());
                successor->setRight(node->getRight());
                successor->getRight()->setParent(successor);
            }
           
            shift(node, successor);        
            successor->setLeft(node->getLeft());
            successor->getLeft()->setParent(successor);
        }
        delete node;
    }


    /**
     * Returns the successor node of the given node.
     */ 
    Node *successor(Node *node) {
        if (node->getRight() != nullptr) {
            return minimum(node->getRight());
        }
        Node *parent = node->getParent();
        while (parent != nullptr && node == parent->getRight()) {
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    }    
    

    /**
     * Returns the predecessor node of the given node.
     */
    Node *predecessor(Node *node) {
        if (node->getLeft() != nullptr) {
            return maximum(node->getLeft());
        }
        Node *parent = node->getParent();
        while (parent != nullptr && node == parent->getLeft()) {
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    } 


    /**
     * Returns the maximum of the node's subtree.
     */
    Node *maximum(Node *node) {
        while (node->getRight() != nullptr) {
            node = node->getRight();
        }
        return node;
    }

    /**
     * Returns the minimum of the node's subtree.
     */
    Node *minimum(Node *node) {
        while (node->getLeft() != nullptr) {
            node = node->getLeft();
        }
        return node;
    }

    /**
     * Finds the node with the given key.
     * Precondition: Node with key exists.
     */ 
    Node *findNode(int key) {
        Node *current = root;
        bool found = false;

        while (!found && current != nullptr) {
            if (key == current->getKey()) {
                found = true;
            } else if (key < current->getKey()) {
                current = current->getLeft();
            } else {
                current = current->getRight();
            }
        }

        return current;
    }

    void inorderPrint() {
        inorderWalk(root);
    }


    void inorderWalk(Node *node) {
        if (node != nullptr) {
            inorderWalk(node->getLeft());
            std::cout << "(" << node->getKey() << ", " << node->getValue() << ")" << std::endl;
            inorderWalk(node->getRight());
        }
    }

    void test() {
        std::cout << root->getKey() << std::endl;
        std::cout << root->getLeft()->getKey() << std::endl;
        std::cout << root->getRight()->getKey() << std::endl;
    }
};

int main() {
    BinaryTree bt;
    bt.insertNode(5, 2);
    bt.insertNode(4, 1);
    bt.insertNode(7, 20);
    bt.insertNode(10, 4);
    bt.insertNode(103, 30);
    bt.insertNode(235, 4);
    bt.insertNode(23, 4);
    bt.insertNode(5, 4);
    bt.insertNode(15, 4);
    bt.deleteNode(5);
    bt.inorderPrint();
    bt.deleteNode(235);
    bt.inorderPrint();
    bt.deleteNode(10);
    bt.inorderPrint();
    //bt.test();
    return 0;
}