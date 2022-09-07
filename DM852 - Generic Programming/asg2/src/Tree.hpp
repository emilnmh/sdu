#ifndef TREE_H
#define TREE_H

namespace DM852 {
    template <typename Key, typename Value, typename Comp = std::less<Key>>
    class Tree {
    public:
        using key_type = Key;
        using value_type = Value;
        using compare_type = Comp;

        template <typename key_type, typename value_type, typename compare_type = Comp>
        class Node {
        friend class Tree;            
        private:
            const key_type key;
            value_type value;
            Node<key_type, value_type> *parent;
            Node<key_type, value_type> *left;
            Node<key_type, value_type> *right;
            compare_type cmp = compare_type();
        public:
            /**
             * Construct a new Node object
             * 
             * @param key Key of the node, used for searching.
             * @param value Value of the node.
             */
            Node(const key_type key, const value_type &value) : key(key), value(value), parent(nullptr), left(nullptr), right(nullptr) {}


            /**
             * @brief Move construct.
             * 
             * @param key Key of the node.
             * @param value Value of the node.
             */
            Node(key_type &&key, value_type &&value) : key((key_type&&)key), value((value_type&&)value), parent(nullptr), left(nullptr), right(nullptr) {}


            /**
             * Returns the smallest key larger than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return Node* pointer to the node with the smallest larger key node | nullptr.
             */
            Node<key_type, value_type> *next() {
                Node<key_type, value_type> *next = nullptr;
                if (this != nullptr) {
                    // If there is no child bigger than this.
                    if (this->right != nullptr) {
                        // There exists a child with a smaller key.
                        next = this->right->minimum();
                    } else {
                        // Find the first parent which path goes to the left to this and it has a right child.
                        bool foundGreater = false;
                        next = this;
                        while (next->parent != nullptr && !foundGreater) {
                            if (next->parent->left == next) {
                                next = next->parent;
                                foundGreater = true;
                            } else {
                                next = next->parent;
                            }
                        }
                        if (!foundGreater)
                            next = nullptr;
                    }
                }
                return next;
            }


            /**
             * Returns the smallest key larger than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return const Node* pointer to the node with the smallest larger key node | nullptr.
             */
            const Node<Key, Value> *next() const {
                const Node<key_type, value_type> *next = nullptr;
                if (this != nullptr) {
                    // If there is no child bigger than this.
                    if (this->right != nullptr) {
                        // There exists a child with a smaller key.
                        next = this->right->minimum();
                    } else {
                        // Find the first parent which path goes to the left to this and it has a right child.
                        bool foundGreater = false;
                        next = this;
                        while (next->parent != nullptr && !foundGreater) {
                            if (next->parent->left == next) {
                                next = next->parent;
                                foundGreater = true;
                            } else {
                                next = next->parent;
                            }
                        }

                        if (!foundGreater)
                            next = nullptr;
                    }
                }
                return next;
            }


            /**
             * Returns the largest key smaller than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return Node* pointer to the node with the largest smaller key node | nullptr.
             */
            Node<Key, Value> *prev() {
                Node<key_type, value_type> *prev = nullptr;
                if (this != nullptr) {
                    // If there is no child bigger than this.
                    if (this->left != nullptr) {
                        // There exists a child with a smaller key.
                        prev = this->left->maximum();
                    } else {
                        // Find the first parent which path goes to the left to this and it has a right child.
                        bool foundLess = false;
                        prev = this;
                        while (prev->parent != nullptr && !foundLess) {
                            if (prev->parent->right == prev) {
                                prev = prev->parent;
                                foundLess = true;
                            } else {
                                prev = prev->parent;
                            }
                        }

                        if (!foundLess)
                            prev = nullptr;
                    }
                }
                return prev;
            }


            /**
             * Returns the largest key smaller than the key in this node.
             * If the node itself has the largest key, nullptr is returned.
             * Computational Complexity: O(n), n = size().
             * 
             * @return const Node* pointer to the node with the largest smaller key node | nullptr.
             */
            const Node<Key, Value> *prev() const {
                const Node<key_type, value_type> *prev = nullptr;
                if (this != nullptr) {
                    // If there is no child bigger than this.
                    if (this->left != nullptr) {
                        // There exists a child with a smaller key.
                        prev = this->left->maximum();
                    } else {
                        // Find the first parent which path goes to the left to this and it has a right child.
                        bool foundLess = false;
                        prev = this;
                        while (prev->parent != nullptr && !foundLess) {
                            if (prev->parent->right == prev) {
                                prev = prev->parent;
                                foundLess = true;
                            } else {
                                prev = prev->parent;
                            }
                        }

                        if (!foundLess)
                            prev = nullptr;
                    }
                }
                return prev;
            }


            /**
             * Returns the child with the smallest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return Node* child node with the smallest key.
             */
            Node<Key, Value> *minimum() {
                Node<Key, Value> *node = this;
                // Go back down all the way to the left.
                while (node->left != nullptr) {
                    node = node->left;
                }
                return node;
            }


            /**
             * Returns the child with the smallest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return const Node* child node with the smallest key.
             */
            const Node<Key, Value> *minimum() const {
                const Node<Key, Value> *node = this;
                // Go back down all the way to the left.
                while (node->left != nullptr) {
                    node = node->left;
                }
                return node;
            }


            /**
             * Returns the child with the largest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return Node* child node with the largest key.
             */
            Node<Key, Value> *maximum() {
                Node<Key, Value> *node = this;
                // Go back down all the way to the right.
                while (node->right != nullptr) {
                    node = node->right;
                }
                return node;
            }


            /**
             * Returns the child with the largest key.
             * Computational Complexity: O(h), where h is the height of the sub-tree with this node as the root.
             * 
             * @return const Node* child node with the largest key.
             */
            const Node<Key, Value> *maximum() const {
                const Node<Key, Value> *node = this;
                // Go back down all the way to the right.
                while (node->right != nullptr) {
                    node = node->right;
                }
                return node;
            }
        };


        /**
         * const_iterator for the Tree class.
         */
        class Const_Tree_Iterator {
        friend class Tree;
        private:
            Node<key_type, value_type> *n_ptr;
            const Tree &tree;
        public:
            using pointer = value_type*;
            using reference = const value_type&;
            using difference_type = ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            /**
             * @brief Construct a new Const_Tree_Iterator object.
             * 
             * @param n Node of the iterator.
             * @param t Tree that the node is in.
             */
            Const_Tree_Iterator(Node<key_type, value_type> *n, const Tree<key_type, value_type, Comp> &t) : n_ptr(n), tree(t) {}


            /**
             * @brief Construct the past-end iterator.
             * 
             * @param t The tree that the iterator iterates over.
             */
            Const_Tree_Iterator(const Tree<key_type, value_type, Comp> &t) : n_ptr(nullptr), tree(t) {}


            /**
             * Returns the value of the iterator.
             * 
             * @return reference 
             */
            reference operator*() const {
                return n_ptr->value;
            }


            /**
             * Pre-increment the iterator.
             * 
             * @pre Iterator isn't the the past-end iterator.
             * 
             * @return Const_Tree_Iterator& 
             */
            Const_Tree_Iterator &operator++() {
                this->n_ptr = n_ptr->next();
                return *this;
            }
            

            /**
             * Post-increment the iterator.
             * 
             * @pre Iterator isn't the the past-end iterator.
             * 
             * @return Const_Tree_Iterator& 
             */
            Const_Tree_Iterator operator++(int) {
                Const_Tree_Iterator it = *this;
                ++(*this);
                return it;
            }


            /**
             * Pre-decrement the iterator.
             * Computational complexity: o(h), h is the height of the tree.
             * 
             * @pre Isn't the first iterator of the tree.
             * 
             * @return Const_Tree_Iterator& 
             */
            Const_Tree_Iterator &operator--() {
                if (this->n_ptr != nullptr) {
                    this->n_ptr = n_ptr->prev();
                } else {
                    this->n_ptr = tree.last;
                }
                return *this;
            }


            /**
             * Post-decrement the iterator.
             * Computational complexity: o(h), h is the height of the tree.
             * 
             * @pre Isn't the first iterator of the tree.
             * 
             * @return Const_Tree_Iterator& 
             */
            Const_Tree_Iterator operator--(int) {
                Const_Tree_Iterator it = *this;
                ++(*this);
                return it;
            }


            /**
             * Compares equality between iterators.
             * 
             * @param other The other iterator that this is compared to.
             * @return true if they are equal, otherwise false.
             */
            bool operator==(const Const_Tree_Iterator &other) const {
                return !(*this != other);
            }


            /**
             * Compares inequality between iterators.
             * 
             * @param other The other iterator that this is compared to.
             * @return true if they are not equal, otherwise false.
             */
            bool operator!=(const Const_Tree_Iterator &other) const {
                return n_ptr != other.n_ptr;
            }
        };


        class Tree_Iterator {
        friend class Tree;
        private:
            Node<key_type, value_type> *n_ptr;
            Tree &tree;
        public:
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;


            /**
             * @brief Construct a new Tree_Iterator object.
             * 
             * @param n Node of the iterator.
             * @param t Tree that the node is in.
             */
            Tree_Iterator(Node<key_type, value_type> *n, Tree<key_type, value_type, Comp> &t) : n_ptr(n), tree(t) {}


            /**
             * @brief Construct the past-end iterator.
             * 
             * @param t The tree that the iterator iterates over.
             */
            Tree_Iterator(Tree<key_type, value_type, Comp> &t) : n_ptr(nullptr), tree(t) {}


            /**
             * Returns the value of the iterator.
             * 
             * @return reference 
             */
            reference operator*() const {
                return n_ptr->value;
            }


            /**
             * Pre-increment the iterator.
             * 
             * @pre Iterator isn't the the past-end iterator.
             * 
             * @return Const_Tree_Iterator& 
             */
            Tree_Iterator &operator++() {
                this->n_ptr = n_ptr->next();
                return *this;
            }


            /**
             * Post-increment the iterator.
             * 
             * @pre Iterator isn't the the past-end iterator.
             * 
             * @return Const_Tree_Iterator& 
             */
            Tree_Iterator operator++(int) {
                Tree_Iterator it = *this;
                ++(*this);
                return it;
            }


            /**
             * Pre-decrement the iterator.
             * 
             * @pre Isn't the first iterator of the tree.
             * 
             * @return Const_Tree_Iterator& 
             */
            Tree_Iterator &operator--() {
                if (this->n_ptr != nullptr) {
                    this->n_ptr = n_ptr->prev();
                } else {
                    this->n_ptr = tree.last;
                }
                return *this;
            }


            /**
             * Post-decrement the iterator.
             * 
             * @pre Isn't the first iterator of the tree.
             * 
             * @return Const_Tree_Iterator& 
             */
            Tree_Iterator operator--(int) {
                Tree_Iterator it = *this;
                --(*this);
                return it;
            }


            /**
             * Compares equality between iterators.
             * 
             * @param other The other iterator that this is compared to.
             * @return true if they are equal, otherwise false.
             */
            bool operator==(const Tree_Iterator &other) const {
                return !(*this != other);
            }


            /**
             * Compares inequality between iterators.
             * 
             * @param other The other iterator that this is compared to.
             * @return true if they are not equal, otherwise false.
             */
            bool operator!=(const Tree_Iterator &other) const {
                return n_ptr != other.n_ptr;
            }


            /**
             * Converts iterator to const_iterator.
             * 
             * @return Const_Tree_Iterator 
             */
            operator Const_Tree_Iterator() const {
                return Const_Tree_Iterator(this->n_ptr, this->tree);
            }
        };
    private:
        Node<key_type, value_type> *root = nullptr;
        Node<key_type, value_type> *first = nullptr;
        Node<key_type, value_type> *last = nullptr;
        int elements = 0;
        compare_type cmp = compare_type();


        /**
         * Used for deletion, to move subtrees around.
         * 
         * @param parent Parent node.
         * @param child Child node.
         */
        void transplant(const Node<key_type, value_type> *parent, Node<key_type, value_type> *child) {
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
         * Recursive deletes the node with all its children.
         * Computational Complexity: O(n), where n is the size of the sub-tree with the given node as the root. 
         * 
         * @param node The node to be deleted.
         */
        void recursive_delete(Node<key_type, value_type> *node) {
            if (node) {
                recursive_delete(node->left);
                recursive_delete(node->right);
                delete node;
            }
        }


        /**
         * Recursively inserts nodes such that a copy can be made.
         * Computational Complexity: O(n), where n is the size of the sub-tree with the given node as the root. 
         * 
         * @param other The node from the other tree.
         * @param parent The parent of the new node.
         * @return Node* The new node | nullptr if other was nullptr.
         */
        Node<key_type, value_type> *recursive_insert(const Node<key_type, value_type> *other, Node<key_type, value_type> *parent = nullptr) {
            if (other != nullptr) {
                Node<key_type, value_type> *node = new Node(other->key, other->value);
                node->parent = parent;
                node->left = recursive_insert(other->left, node);
                node->right = recursive_insert(other->right, node);
                return node;
            }
            return nullptr;
        }

    public:
        using iterator = Tree_Iterator;
        using const_iterator = Const_Tree_Iterator;


        /**
         * Construct a new Tree object
         */
        Tree() = default;


        /**
         * Construct a new Tree object with a specific comparator.
         * 
         * @param c Comparator to decide the order of the nodes in the tree.
         */
        Tree(compare_type c) : cmp(c) {}


        /**
         * Copy constructor.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other Tree to make a copy of.
         */
        Tree(const Tree &other) {
            const_iterator it = other.begin();
            this->root = recursive_insert(it.n_ptr, nullptr);
            this->elements = other.size();
            this->cmp = other.cmp;
        }


        /**
         * Move constructor.
         * 
         * @param other
         */
        Tree(Tree<Key, Value> &&other) : root(nullptr), elements(0), cmp(other.cmp) { 
            *this = std::move(other);
        }


        /**
         * Destroy the Tree object
         * Computational Complexity: O(n), n = size().
         */
        ~Tree() {
            recursive_delete(this->root);
        }


        /**
         * Returns the number of elements stored.
         * 
         * @return int 
         */
        int size() const {
            return this->elements;
        }


        /**
         * Return true iff the container is empty.
         * 
         * @return true iff the container is empty.
         */
        bool empty() const {
            return this->size() == 0;
        }


        /**
         * Insert a new node or update an existing node in the tree with the same key.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key Key of the node to insert. 
         * @param value Value of the node to insert.
         * @return std::pair<Node*, bool> the pair with the inserted node and boolean value if true
         * a node was inserted, or false if a node was updated instead.
         */
        std::pair<Node<key_type, value_type>*, bool> insert(const key_type key, const value_type &value) {
            Node<key_type, value_type> *y = nullptr;
            Node<key_type, value_type> *x = this->root;
            bool existingKey = false;
            while (x != nullptr && !existingKey) {
                y = x;
                if (cmp(key, x->key)) { // Has to be inserted in left sub-tree.
                    x = x->left;
                } else if (cmp(x->key, key)) { // Has to be inserted in right sub-tree.
                    x = x->right;
                } else { // Found node with the same key.
                    existingKey = true;
                }
            }
            std::pair<Node<key_type, value_type>*, bool> result;
            if (!existingKey) {
                Node<key_type, value_type> *newNode = new Node(key, value);
                newNode->parent = y;
                if (y == nullptr) { // If the tree was empty.
                    this->root = newNode;
                } else if (cmp(key, y->key)) { // New node is left child.
                    y->left = newNode;
                } else { // New node is right child.
                    y->right = newNode; 
                }
                this->elements += 1;
                result.first = newNode;
                result.second = true;
                if (this->first == nullptr || cmp(key, this->first->key)) this->first = newNode;
                if (this->last == nullptr || cmp(this->last->key, key)) this->last = newNode;
            } else { // If there already was a node with the key.
                x->value = value;
                result.first = x;
                result.second = false;
            }
            return result;
        }

        
        /**
         * Move insert a new node or update an existing node in the tree with the same key.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key Key of the node to insert. 
         * @param value Value of the node to insert.
         * @return std::pair<Node*, bool> the pair with the inserted node and boolean value if true
         * a node was inserted, or false if a node was updated instead.
         */
        std::pair<Node<key_type, value_type>*, bool> insert(key_type &&key, value_type &&value) {
            Node<key_type, value_type> *y = nullptr;
            Node<key_type, value_type> *x = this->root;
            bool existingKey = false;
            while (x != nullptr && !existingKey) {
                y = x;
                if (cmp(key, x->key)) { // Has to be inserted in left sub-tree.
                    x = x->left;
                } else if (cmp(x->key, key)) { // Has to be inserted in right sub-tree.
                    x = x->right;
                } else { // Found node with the same key.
                    existingKey = true;
                }
            }
            std::pair<Node<key_type, value_type>*, bool> result;
            if (!existingKey) {
                Node<key_type, value_type> *newNode = new Node(std::move(key), std::move(value));
                newNode->parent = y;
                if (y == nullptr) { // If the tree was empty.
                    this->root = newNode;
                } else if (cmp(key, y->key)) { // New node is left child.
                    y->left = newNode;
                } else { // New node is right child.
                    y->right = newNode; 
                }
                this->elements += 1;
                result.first = newNode;
                result.second = true;
                if (this->first == nullptr || cmp(key, this->first->key)) this->first = newNode;
                if (this->last == nullptr || cmp(this->last->key, key)) this->last = newNode;
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
         * Computational Complexity: O(n), n = size().
         * 
         * @param key The key the node should have.
         * @return Node* Node with the key, or nullptr if there is no node with the key.
         */
        iterator find(const key_type key) {
            Node<key_type, value_type> *node = this->root;
            bool found = false;
            while (node != nullptr && !found) {
                if (cmp(key, node->key)) {
                    node = node->left;
                } else if (cmp(node->key, key)) {
                    node = node->right;
                } else {
                    found = true;
                }
            }
            return iterator(node, *this);
        }


        /**
         * Return the node with the key.
         * Returns nullptr if there's no Node with the key.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key The key the node should have.
         * @return const Node* Node with the key, or nullptr if there is no node with the key.
         */
        const_iterator find(const key_type key) const {
            const Node<key_type, value_type> *node = this->root;
            bool found = false;
            while (node != nullptr && !found) {
                if (cmp(key, node->key)) {
                    node = node->left;
                } else if (cmp(node->key, key)) {
                    node = node->right;
                } else {
                    found = true;
                }
            }
            return const_iterator(node, *this);
        }


        /**
         * Erase all elements.
         * Computational Comlexity: O(n), n = size().
         */
        void clear() {
            Node<key_type, value_type> *node = this->root;
            if (node != nullptr) {
                recursive_delete(node);
                this->elements = 0;
                this->root = nullptr;
                this->first = nullptr;
                this->last = nullptr;
            }
        }


        /**
         * Erase the key-value pair with the given key, or do nothing if no such key exists.
         * Computational Complexity: O(n), n = size().
         * 
         * @param key The key the node should have.
         */
        void erase(const key_type key) {
            erase(find(key));
        }


        /**
         * Erase the element stored in node.
         * Computational complexity: O(h), h is the height of the tree.
         * @pre node is a node from the tree.
         * 
         * @param node the node to be deleted.
         */
        void erase(const_iterator it) {
            // If a node was found.
            if (it.n_ptr != nullptr) {        
                // Set the new first.
                if (it.n_ptr == this->first) {
                    if (it.n_ptr->right != nullptr) this->first = it.n_ptr->right->minimum();
                    else if (it.n_ptr->parent != nullptr) this->first = it.n_ptr->parent;
                    else this->first = nullptr;
                }

                // Set the new last.
                if (it.n_ptr == this->last) {
                    if (it.n_ptr->left != nullptr) this->last = it.n_ptr->left->maximum();
                    else if (it.n_ptr->parent != nullptr) this->last = it.n_ptr->parent;
                    else this->last = nullptr;
                }
                
                // Move the nodes around.
                if (it.n_ptr->left == nullptr)
                    transplant(it.n_ptr, it.n_ptr->right);
                else if (it.n_ptr->right == nullptr)
                    transplant(it.n_ptr, it.n_ptr->left);
                else {
                    Node<key_type, value_type> *y = it.n_ptr->right->minimum();
                    if (y->parent != it.n_ptr) {
                        transplant(y, y->right);
                        y->right = it.n_ptr->right;
                        y->right->parent = y;
                    }
                    transplant(it.n_ptr, y);
                    y->left = it.n_ptr->left;
                    y->left->parent = y;
                }
                delete it.n_ptr;
                this->elements -= 1;
            }
        }


        /**
         * The first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return value_type& the first element. 
         */
        value_type &front() {
            return this->first->value;
        }


        /**
         * The first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return const value_type& The first element.
         */
        const value_type &front() const {
            return this->first->value;
        }


        /**
         * The last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return value_type& The last element.
         */
        value_type &back() {
            return this->last->value;
        }


        /**
         * The last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @pre size() > 0.
         * 
         * @return const value_type& The last element.
         */
        const value_type &back() const {
            return this->last->value;
        }


        /**
         * The node storing the first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return Node* The node stroing the first element.
         */
        iterator begin() {
            return iterator(this->root, *this);
        }


        /**
         * The node storing the first element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return const Node<key_type, value_type>* The node stroing the first element.
         */
        const_iterator begin() const {
            return const_iterator(this->root, *this);
        }


        /**
         * The node storing the last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return Node<key_type, value_type>* The node storing the last element, always nullptr.
         */
        iterator end() {
            return iterator(*this);
        }

        /**
         * The node storing the last element.
         * Computational Complexity: O(h), where h is the height of the tree.
         * 
         * @return const Node* The node storing the last element, always nullptr.
         */
        const_iterator end() const {
            return const_iterator(*this);
        }


        /**
         * Compares equality between this and other tree.
         * Checks if in the order of the nodes if they have the same key and value.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other The other tree to compare to.
         * @return true | false
         */
        bool operator==(const Tree &other) const {
            // Checks if the trees have the same size.
            if (other.size() == this->size()) {
                const_iterator thisIt = this->begin();
                const_iterator otherIt = other.begin();
                while (thisIt != this->end() && *thisIt == *otherIt) {
                    thisIt++;
                    otherIt++;
                }
                return thisIt == this->end() && otherIt == other.end();
            }
            return false;
        }


        /**
         * Compares inequality between this and other tree.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other Other tree to compare to.
         * @return true | false
         */
        bool operator!=(const Tree<key_type, value_type, Comp> &other) const {
            return !(*this == other);
        }


        /**
         * Makes a copy of the other tree.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other Other tree that the copy should be made of.
         * @return Tree& The new tree.
         */
        Tree<key_type, value_type, Comp> &operator=(const Tree<key_type, value_type, Comp> &other) {
            const_iterator it = other.begin();
            this->root = recursive_insert(it.n_ptr, nullptr);
            this->elements = other.size();
            return *this;
        }


        /**
         * Move assignment.
         * 
         * @param other The other tree to move the data from.
         * @return Tree<key_type, value_type, Comp>& this.
         */
        Tree<key_type, value_type, Comp> &operator=(Tree<key_type, value_type, Comp> &&other) {
            this->root = other.root;
            this->elements = other.elements;
            this->cmp = other.cmp;
            other.root = nullptr;
            other.elements = 0;
            return *this;
        }
    };
}

#endif