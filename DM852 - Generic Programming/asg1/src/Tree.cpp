#include "Tree.hpp"
#include <iostream>

using namespace DM852;
using namespace TREE_NODE;


/**
 * Construct a new Node:: Node object
 * 
 * @param key Key of the node, used for searching.
 * @param value Value of the node.
 */
Node::Node(int key, const std::string &value) : key(key), value(value), parent(nullptr), left(nullptr), right(nullptr) {}


/**
 * Returns the smallest key larger than the key in this node.
 * If the node itself has the largest key, nullptr is returned.
 * 
 * @return Node* pointer to the node with the smallest larger key node | nullptr.
 */
Node *Node::next() {
    Node *next = nullptr;
    if (this != nullptr) {
        bool noNext = false;
        
        // If there is no child bigger than this.
        if (this->maximum() == this) {
            next = this;
            while (next->key <= this->key && !noNext) {
                if (next->parent != nullptr) 
                    next = next->parent;
                else 
                    noNext = true;
            }

            if (next->parent == nullptr) {
                noNext = true;
            }
        } else {
            next = this;
        }

        if (!noNext) {
            // Find the smallest bigger key.
            if (next->right != nullptr) {
                next = next->right;
                while (next != nullptr && next->left != nullptr) {
                    next = this->left;
                }
            } else {
                next = nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return next;
}


/**
 * Returns the smallest key larger than the key in this node.
 * If the node itself has the largest key, nullptr is returned.
 * 
 * @return const Node* pointer to the node with the smallest larger key node | nullptr.
 */
const Node *Node::next() const {
    const Node *next = nullptr;
    if (this != nullptr) {
        bool noNext = false;

        // If there is no child bigger than this.
        if (this->maximum() == this) {
            next = this;
            while (next->key <= this->key && !noNext) {
                if (next->parent != nullptr) 
                    next = next->parent;
                else 
                    noNext = true;
            }

            if (next->parent == nullptr) {
                noNext = true;
            }
        } else {
            next = this;
        }

        if (!noNext) {
            // Find the smallest bigger key.
            if (next->right != nullptr) {
                next = next->right;
                while (next != nullptr && next->left != nullptr) {
                    next = this->left;
                }
            } else {
                next = nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return next;
}


/**
 * Returns the largest key smaller than the key in this node.
 * If the node itself has the largest key, nullptr is returned.
 * 
 * @return Node* pointer to the node with the largest smaller key node | nullptr.
 */
Node *Node::prev() {
    Node *prev = nullptr;
    if (this != nullptr) {
        bool noPrev = false;
        
        // If there is no child bigger than this.
        if (this->minimum() == this) {
            prev = this;
            while (prev->key <= this->key && !noPrev) {
                if (prev->parent != nullptr) 
                    prev = prev->parent;
                else 
                    noPrev = true;
            }

            if (prev->parent == nullptr) {
                noPrev = true;
            }
        } else {
            prev = this;
        }

        if (!noPrev) {
            // Find the smallest bigger key.
            if (prev->right != nullptr) {
                prev = prev->right;
                while (prev != nullptr && prev->left != nullptr) {
                    prev = this->left;
                }
            } else {
                prev = nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return prev;
}


/**
 * Returns the largest key smaller than the key in this node.
 * If the node itself has the largest key, nullptr is returned.
 * 
 * @return const Node* pointer to the node with the largest smaller key node | nullptr.
 */
const Node *Node::prev() const {
    const Node *prev;
    if (this != nullptr) {
        bool noPrev = false;
        
        // If there is no child bigger than this.
        if (this->minimum() == this) {
            prev = this;
            while (prev->key <= this->key && !noPrev) {
                if (prev->parent != nullptr) 
                    prev = prev->parent;
                else 
                    noPrev = true;
            }

            if (prev->parent == nullptr) {
                noPrev = true;
            }
        } else {
            prev = this;
        }

        if (!noPrev) {
            // Find the smallest bigger key.
            if (prev->right != nullptr) {
                prev = prev->right;
                while (prev != nullptr && prev->left != nullptr) {
                    prev = this->left;
                }
            } else {
                prev = nullptr;
            }
        } else {
            return nullptr;
        }
    }
    return prev;
}


/**
 * Returns the child with the smallest key.
 * 
 * @return Node* child node with the smallest key.
 */
Node *Node::minimum() {
    Node *node = this;
    // Go back down all the way to the left.
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}


/**
 * Returns the child with the smallest key.
 * 
 * @return const Node* child node with the smallest key.
 */
const Node *Node::minimum() const {
    const Node *node = this;
    // Go back down all the way to the left.
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}


/**
 * Returns the child with the largest key.
 * 
 * @return Node* child node with the largest key.
 */
Node *Node::maximum() {
    Node *node = this;
    // Go back down all the way to the right.
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}


/**
 * Returns the child with the largest key.
 * 
 * @return const Node* child node with the largest key.
 */
const Node *Node::maximum() const {
    const Node *node = this;
    // Go back down all the way to the right.
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}


/**
 * Constructor.
 */
Tree::Tree() = default;


/**
 * Copy constructor.
 * 
 * @param other Tree to make a copy of.
 */
Tree::Tree(const Tree &other) {
    const Node *current = other.begin();
    this->root = recursive_insert(current, nullptr);
    this->elements = other.size();
}


/**
 * Destructor.
 */
Tree::~Tree() {
    recursive_delete(this->root);
}


/**
 * Recusively deletes the node with all its children.
 * 
 * @param node Node to be deleted.
 */
void Tree::recursive_delete(Node *node) {
    if (node) {
        recursive_delete(node->left);
        recursive_delete(node->right);
        delete node;
    }
}


/**
 * Returns the number of elements stored.
 * 
 * @return int 
 */
int Tree::size() const {
    return this->elements;
}


/**
 * Return true iff the container is empty.
 * 
 * @return true iff the container is empty.
 */
bool Tree::empty() const {
    return this->size() == 0;
}


/**
 * Insert a new node or update an existing node in the tree with the same key.
 * 
 * @param key Key of the node to insert. 
 * @param value Value of the node to insert.
 * @return std::pair<Node*, bool> the pair with the inserted node and boolean value if true
 * a node was inserted, or false if a node was updated instead.
 */
std::pair<Node*, bool> Tree::insert(int key, const std::string &value) {
    Node *y = nullptr;
    Node *x = this->root;

    bool existingKey = false;
    while (x != nullptr && !existingKey) {
        y = x;
        if (key < x->key) { // Has to be inserted in left sub-tree.
            x = x->left;
        } else if (key > x->key) { // Has to be inserted in right sub-tree.
            x = x->right;
        } else { // Found node with the same key.
            existingKey = true;
        }
    }
    
    std::pair<Node*, bool> result;

    if (!existingKey) {
        Node *newNode = new Node(key, value);
        newNode->parent = y;
        
        if (y == nullptr) { // If the tree was empty.
            this->root = newNode;
        } else if (key < y->key) { // New node is left child.
            y->left = newNode;
        } else { // New node is right child.
            y->right = newNode; 
        }

        this->elements += 1;

        result.first = newNode;
        result.second = true;
    } else { // If there already was a node with the key.
        x->value = value;
        result.first = x;
        result.second = false;
    }

    return result;
}


/**
 * Return the node with the key.
 * Returns nullptr if there's no Node with the key.
 * 
 * @param key The key the node should have.
 * @return Node* Node with the key, or nullptr if there is no node with the key.
 */
Node *Tree::find(int key) {
    Node *node = this->root;
    bool found = false;
    while (node != nullptr && !found) {
        if (key < node->key) {
            node = node->left;
        } else if (key > node->key) {
            node = node->right;
        } else {
            found = true;
        }
    }
    return node;
}


/**
 * Return the node with the key.
 * Returns nullptr if there's no Node with the key.
 * 
 * @param key The key the node should have.
 * @return const Node* Node with the key, or nullptr if there is no node with the key.
 */
const Node *Tree::find(int key) const {
    return find(key);
}


/**
 * Erase all elements.
 */
void Tree::clear() {
    Node *node = this->root;
    if (node != nullptr) {
        recursive_delete(node);
        this->elements = 0;
        this->root = nullptr;
    }
}


/**
 * Used for deletion, to move subtrees around.
 * 
 * @param parent Parent node.
 * @param child Child node.
 */
void Tree::transplant(const Node *parent, Node *child) {
    if (parent->parent == nullptr)
        this->root = child;
    else if (parent->parent->left == parent)
        parent->parent->left = child;
    else
        parent->parent->right = child;
    
    if (child != nullptr)   
        child->parent = parent->parent;
}


/**
 * Erase the key-value pair with the given key, or do nothing if no such key exists.
 * 
 * @param key The key the node should have.
 */
void Tree::erase(int key) {
    erase(find(key));
}


/**
 * Erase the element stored in node.
 * @pre node is a node from the tree.
 * 
 * @param node the node to be deleted.
 */
void Tree::erase(const Node *node) {
    // If a node was found.
    if (node != nullptr) {        
        if (node->left == nullptr)
            transplant(node, node->right);
        else if (node->right == nullptr)
            transplant(node, node->left);
        else {
            Node *y = node->right->minimum();
            if (y->parent != node) {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }
            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
        }
        
        delete node;
        this->elements -= 1;
    }
}


/**
 * The first element.
 * @pre size() > 0.
 * 
 * @return std::string& the first element. 
 */
std::string &Tree::front() {
    return root->minimum()->value;
}


/**
 * The first element.
 * @pre size() > 0.
 * 
 * @return const std::string& The first element.
 */
const std::string &Tree::front() const {
    return root->minimum()->value;
}


/**
 * The last element.
 * @pre size() > 0.
 * 
 * @return std::string& The last element.
 */
std::string &Tree::back() {
    return root->maximum()->value;
}


/**
 * The last element.
 * @pre size() > 0.
 * 
 * @return const std::string& The last element.
 */
const std::string &Tree::back() const {
    return root->maximum()->value;
}


/**
 * The node storing the first element.
 * 
 * @return Node* The node stroing the first element.
 */
Node *Tree::begin() {
    Node *node = this->root;
    if (node != nullptr) {
        node = root->minimum();
    }
    return node;
}


/**
 * The node storing the first element.
 * 
 * @return const Node* The node storing the first element.
 */
const Node *Tree::begin() const {
    Node *node = this->root;
    if (node != nullptr) {
        node = root->minimum();
    }
    return node;
}


/**
 * The node storing the last element.
 * 
 * @return Node* The node storing the last element, always nullptr.
 */
Node *Tree::end() {
    return nullptr;
}


/**
 * The node storing the last element.
 * 
 * @return const Node* The node storing the last element, always nullptr.
 */
const Node *Tree::end() const {
    return nullptr;
}


/**
 * Compares equality between this and other tree.
 * Checks if in the order of the nodes if they have the same key and value.
 * 
 * @param other The other tree to compare to.
 * @return true | false
 */
bool Tree::operator==(const Tree &other) const {
    // Checks if the trees have the same size.
    if (other.size() == this->size()) {
        if (this->size() != 0) {
            // Goes through all the nodes iteratively and compares them.
            const Node *thisNode = this->begin();
            const Node *otherNode = other.begin();
            
            while (thisNode != this->end()) {
                //std::cout << (thisNode->key != otherNode->key || thisNode->value.compare(otherNode->value) != 0) << std::endl;
                if (thisNode->key != otherNode->key || thisNode->value.compare(otherNode->value) != 0)
                    return false;
                thisNode = thisNode->next();
                otherNode = otherNode->next();
            }
        }
        return true;
    }
    return false;
}


/**
 * Compares inequality between this and other tree.
 * 
 * @param other Other tree to compare to.
 * @return true | false
 */
bool Tree::operator!=(const Tree &other) const {
    return !(*this == other);
}


/**
 * Makes a copy of the other tree.
 * 
 * @param other Other tree that the copy should be made of.
 * @return Tree& The new tree.
 */
Tree &Tree::operator=(const Tree &other) {
    const Node *current = other.begin();
    this->root = recursive_insert(current, nullptr);
    this->elements = other.size();
    return *this;
}


/**
 * Recursively inserts nodes such that a copy can be made.
 * 
 * @param other The node from the other tree.
 * @param parent The parent of the new node.
 * @return Node* The new node | nullptr if other was nullptr.
 */
Node *Tree::recursive_insert(const Node *other, Node *parent = nullptr) {
    if (other != nullptr) {
        Node *node = new Node(other->key, other->value);
        node->parent = parent;
        node->left = recursive_insert(other->left, node);
        node->right = recursive_insert(other->right, node);
        return node;
    }
    return nullptr;
}