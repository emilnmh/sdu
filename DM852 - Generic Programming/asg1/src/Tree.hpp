#ifndef TREE_H
#define TREE_H

#include <string>

namespace DM852 {
    namespace TREE_NODE {
        class Node {
        public:
            const int key;
            std::string value;
            Node *parent;
            Node *left;
            Node *right;

            /**
             * @brief Construct a new Node object
             * 
             * @param key Key of the node, used for searching.
             * @param value Value of the node.
             */
            Node(int key, const std::string &value);

            /**
             * Returns the smallest key larger than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return Node* pointer to the node with the smallest larger key node | nullptr.
             */
            Node *next();

            /**
             * Returns the smallest key larger than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return const Node* pointer to the node with the smallest larger key node | nullptr.
             */
            const Node *next() const;

            /**
             * Returns the largest key smaller than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return Node* pointer to the node with the largest smaller key node | nullptr.
             */
            Node *prev();

            /**
             * Returns the largest key smaller than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return const Node* pointer to the node with the largest smaller key node | nullptr.
             */
            const Node* prev() const;

            /**
             * Returns the child with the smallest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return Node* child node with the smallest key.
             */
            Node *minimum();

            /**
             * Returns the child with the smallest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return const Node* child node with the smallest key.
             */
            const Node *minimum() const;

            /**
             * Returns the child with the largest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return Node* child node with the largest key.
             */
            Node *maximum();

            /**
             * Returns the child with the largest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return const Node* child node with the largest key.
             */
            const Node *maximum() const;
        };
    }

    class Tree {
    private:
        TREE_NODE::Node *root = nullptr;
        int elements = 0;

        /**
         * Used for deletion, to move subtrees around.
         * 
         * @param parent Parent node.
         * @param child Child node.
         */
        void transplant(const TREE_NODE::Node *parent, TREE_NODE::Node *child);
        
        /**
         * Recursive deletes the node with all its children.
         * Computational Complexity: O(n), where n is the size of the sub-tree with the given node as the root. 
         * 
         * @param node The node to be deleted.
         */
        void recursive_delete(TREE_NODE::Node *node);

        /**
         * Recursively inserts nodes such that a copy can be made.
         * Computational Complexity: O(n), where n is the size of the sub-tree with the given node as the root. 
         * 
         * @param other The node from the other tree.
         * @param parent The parent of the new node.
         * @return Node* The new node | nullptr if other was nullptr.
         */
        TREE_NODE::Node *recursive_insert(const TREE_NODE::Node *node, TREE_NODE::Node *parent);
    public:
        /**
         * Construct a new Tree object
         */
        Tree();


        /**
         * Copy constructor.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other Tree to make a copy of.
         */
        Tree(const Tree &other);

        /**
         * Destroy the Tree object
         * Computational Complexity: O(n), n = size().
         */
        ~Tree();

        /**
         * Returns the number of elements stored.
         * 
         * @return int 
         */
        int size() const;

        /**
         * Return true iff the container is empty.
         * 
         * @return true iff the container is empty.
         */
        bool empty() const;

        /**
         * Insert a new node or update an existing node in the tree with the same key.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key Key of the node to insert. 
         * @param value Value of the node to insert.
         * @return std::pair<Node*, bool> the pair with the inserted node and boolean value if true
         * a node was inserted, or false if a node was updated instead.
         */
        std::pair<TREE_NODE::Node*, bool> insert(int key, const std::string &value);

        /**
         * Return the node with the key.
         * Returns nullptr if there's no Node with the key.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key The key the node should have.
         * @return Node* Node with the key, or nullptr if there is no node with the key.
         */
        TREE_NODE::Node *find(int key);

        /**
         * Return the node with the key.
         * Returns nullptr if there's no Node with the key.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key The key the node should have.
         * @return const Node* Node with the key, or nullptr if there is no node with the key.
         */
        const TREE_NODE::Node *find(int key) const;

        /**
         * Erase all elements.
         * Computational Comlexity: O(n), n = size().
         */
        void clear();

        /**
         * Erase the key-value pair with the given key, or do nothing if no such key exists.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key The key the node should have.
         */
        void erase(int key);

        /**
         * Erase the element stored in node.
         * @pre node is a node from the tree.
         * 
         * @param node the node to be deleted.
         */
        void erase(const TREE_NODE::Node *node);

        /**
         * The first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return std::string& the first element. 
         */
        std::string &front();

        /**
         * The first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return const std::string& The first element.
         */
        const std::string &front() const;

        /**
         * The last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return std::string& The last element.
         */
        std::string &back();

        /**
         * The last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return const std::string& The last element.
         */
        const std::string &back() const;

        /**
         * The node storing the first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return Node* The node stroing the first element.
         */
        TREE_NODE::Node *begin();

        /**
         * The node storing the first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return const Node* The node stroing the first element.
         */
        const TREE_NODE::Node *begin() const;

        /**
         * The node storing the last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return Node* The node storing the last element, always nullptr.
         */
        TREE_NODE::Node *end();

        /**
         * The node storing the last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return const Node* The node storing the last element, always nullptr.
         */
        const TREE_NODE::Node *end() const;

        /**
         * Compares equality between this and other tree.
         * Checks if in the order of the nodes if they have the same key and value.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other The other tree to compare to.
         * @return true | false
         */
        bool operator==(const Tree &other) const;

        /**
         * Compares inequality between this and other tree.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other Other tree to compare to.
         * @return true | false
         */
        bool operator!=(const Tree &other) const;

        /**
         * Makes a copy of the other tree.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other Other tree that the copy should be made of.
         * @return Tree& The new tree.
         */
        Tree &operator=(const Tree &other);
    };
}

#endif