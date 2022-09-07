#ifndef LIST_H
#define LIST_H

#include <string>
#include <assert.h>

namespace DM852 {
    namespace LIST_NODE {
        template <typename T>
        class Node {
        public:
            T data;
            Node<T> *next = nullptr;
            Node<T> *prev = nullptr;
            Node() = default;
            Node(T &&d) : data((T&&)d) {}
        };
    }

    using namespace LIST_NODE;
    template <typename T>
    class List {   
    private:
        Node<T> *tail; // Past end
        Node<T> *head; // The first element of the list.
        int length = 0; // Number of elements in the list.
    public:    
        class Const_List_Iterator {
        friend class List;
        public:
            using value_type = T;
            using pointer = value_type*;
            using reference = const value_type&;
            using difference_type = ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;
        private:
            Node<value_type> *n_ptr;
        public:


            /**
             * Constructor.
             * 
             * @param n Node the iterator contains.
             */
            Const_List_Iterator(Node<value_type> *n) : n_ptr(n) {}


            /**
             * Constructor.
             */
            Const_List_Iterator() : n_ptr(nullptr) {}


            /**
             * Dereference iterator.
             * 
             * @return const reference& The data.
             */
            reference operator*() const {
                return n_ptr->data;
            }


            /**
             * Post incremental of the iterator.
             * @pre the iterator isn't the past end iterator (List->end()).
             * 
             * @return Const_List_Iterator& The next iterator.
             */
            Const_List_Iterator &operator++() {
                this->n_ptr = n_ptr->next;
                return *this;
            }


            /**
             * Pre incremental of the iterator.
             * @pre the iterator isn't the past end iterator (List->end()).
             * 
             * @return Const_List_Iterator The iterator itself.
             */
            Const_List_Iterator operator++(int) {
                Const_List_Iterator iterator = *this;
                ++(*this);
                return iterator;
            }


            /**
             * Post decremental of the iterator.
             * @pre the iterator isn't the first iterator (list->begin()).
             * 
             * @return Const_List_Iterator& The previous iterator.
             */
            Const_List_Iterator &operator--() {
                this->n_ptr = n_ptr->prev;
                return *this;
            }

            /**
             * Pre decremental of the iterator
             * @pre the iterator isn't the first iterator (list->begin()).
             * 
             * @return Const_List_Iterator The iterator itself.
             */
            Const_List_Iterator operator--(int) {
                Const_List_Iterator iterator = *this;
                --(*this);
                return iterator;
            }


            /**
             * Compares iterators if they are equal.
             * 
             * @param other The other Const_List_Iterator that this should be compared to.
             * @return true if the iterators are equal.
             */
            bool operator==(const Const_List_Iterator &other) const {
                return !(*this != other);
            }


            /**
             * Compares iterators if they are not equal.
             * 
             * @param other The other Const_List_Iterator that this should be compared to.
             * @return true if the iterators are equal.
             */
            bool operator!=(const Const_List_Iterator &other) const {
                return n_ptr != other.n_ptr; 
            }
        };
        
        
        /**
         * Iterator to iterate over the list.
         */
        class List_Iterator {
        friend class List;
        public:
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;
        private:
            Node<value_type> *n_ptr;
        public:

            /**
             * Constructor.
             * 
             * @param n Node the iterator contains.
             */
            List_Iterator(Node<value_type> *n) : n_ptr(n) {}
            

            /**
             * Constructor.
             */
            List_Iterator() : n_ptr(nullptr) {}


            /**
             * Dereference iterator.
             * 
             * @return const reference& The data.
             */
            reference &operator*() const {
                return n_ptr->data;
            }


            /**
             * Post incremental of the iterator.
             * @pre the iterator isn't the past end iterator (List->end()).
             * 
             * @return List_Iterator& The next iterator.
             */
            List_Iterator &operator++() {
                this->n_ptr = n_ptr->next;
                return *this;
            }


            /**
             * Pre incremental of the iterator.
             * @pre the iterator isn't the past end iterator (List->end()).
             * 
             * @return List_Iterator The iterator itself.
             */
            List_Iterator operator++(int) {
                List_Iterator iterator = *this;
                ++(*this);
                return iterator;
            }


            /**
             * Post decremental of the iterator.
             * @pre the iterator isn't the first iterator (list->begin()).
             * 
             * @return List_Iterator& The previous iterator.
             */
            List_Iterator &operator--() {
                this->n_ptr = n_ptr->prev;
                return *this;
            }


            /**
             * Pre decremental of the iterator
             * @pre the iterator isn't the first iterator (list->begin()).
             * 
             * @return List_Iterator The iterator itself.
             */
            List_Iterator operator--(int) {
                List_Iterator iterator = *this;
                --(*this);
                return iterator;
            }


            /**
             * Compares iterators if they are equal.
             * 
             * @param other The other List_Iterator that this should be compared to.
             * @return true if the iterators are equal.
             */
            bool operator==(const List_Iterator &other) const {
                return !(*this != other);
            }


            /**
             * Compares iterators if they are not equal.
             * 
             * @param other The other Const_List_Iterator that this should be compared to.
             * @return true if the iterators are equal.
             */
            bool operator!=(const List_Iterator &other) const {
                return n_ptr != other.n_ptr; 
            }


            /**
             * Conversion function to Const_List_Iterator.
             * 
             * @return Const_List_Iterator This as a Const_List_Iterator.
             */
            operator Const_List_Iterator() const {
                return Const_List_Iterator(this->n_ptr);
            }
        };
        using iterator = List_Iterator;
        using const_iterator = Const_List_Iterator;


        /**
         * Construct a new List object.
         */
        List() {
            this->tail = this->head = new Node<T>();
        }


        /**
         * Implicit conversion from initializer_list to list.
         * Computational complexity: O(n), n = size of il.
         * 
         * @param il The initializer list.
         */
        explicit List(std::initializer_list<T> il) {
            this->tail = this->head = new Node<T>();
            for (auto i = il.begin(); i != il.end(); ++i) {
                this->push_back(*i);
            }
        }


        /**
         * Copy constructor.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other The other list to copy the elements from.
         */
        List(const List<T> &other) {
            // Copy all the nodes.
            this->tail = this->head = new Node<T>();
            for (const_iterator it = other.begin(); it != other.end(); ++it) {
                this->push_back(*it);
            }
        }


        /**
         * Move constructor.
         * 
         * @param other 
         */
        List(List<T> &&other) {
            this->tail = other.tail;
            this->head = other.head;
            this->length = other.length;
            other.tail = other.head = new Node<T>();
            other.length = 0;
        }


        /**
         * Destroy the List object.
         * Computational Complexity: O(n), n = size().
         */
        ~List<T>() {
            Node<T> *current = this->head;
            Node<T> *next;
            for (int i = 0; i <= this->size(); i++) {
                next = current->next;
                delete current;
                current = next;
            }
        }

        /**
         * Return the number of elements stored in the list.
         * 
         * @return int Number of elements stored in the list.
         */
        int size() const {
            return this->length;
        }

        /**
         * Returns true if the list is empty.
         * 
         * @return true | false
         */
        bool empty() const {
            return this->length == 0;
        }


        /**
         * Inserts a new element in the end.
         * 
         * @param elem The element to be inserted.
         */
        void push_back(const T &elem) {
            Node<T> *n = new Node<T>();
            n->data = elem;
            if (!empty()) // If the list is non-empty.
                this->tail->prev->next = n;
            else
                this->head = n;
            n->next = this->tail;
            n->prev = this->tail->prev;
            this->tail->prev = n;
            // Increment size.
            this->length++;
        }


        /**
         * Moves the element to the end.
         * 
         * @param elem The element to be inserted.
         */
        void push_back(T &&elem) {
            Node<T> *n = new Node<T>(std::move(elem));
            if (!empty())
                this->tail->prev->next = n;
            else 
                this->head = n;
            n->next = this->tail;
            n->prev = this->tail->prev;
            this->tail->prev = n;
            // Increment size.
            this->length++;
        }


        /**
         * Erase all elements.
         * Computational Complexity: O(n), n = size().
         */
        void clear() {
            // TODO change to iterator.
            Node<T> *current = this->head; 
            Node<T> *next;
            // Iterate over list and delete elements.
            while (current != this->tail) {
                next = current->next;
                delete current;
                current = next;
            }
            // Reset fields.
            this->length = 0;
            this->tail->prev = nullptr;
            this->head = this->tail;
        }
        

        /**
         * Erase the last element.
         */
        void pop_back() {
            // Set new tail.
            Node<T> *delete_node = this->tail->prev;
            this->tail->prev = delete_node->prev;
            // If it is the last element.
            if (delete_node->prev == nullptr) this->head = this->tail;
            else delete_node->prev->next = this->tail;
            delete delete_node;
            // Decrement size.
            this->length--;
        }


        /**
         * Inserts a new element before the one stored in the node.
         * @pre node != nullptr and node is in the list.
         * 
         * @param node The node that new node should be inserted infront of.
         * @param elem The element that should be inserted.
         * @return Node* 
         */
         iterator insert(const_iterator pos, const T &elem) {
            Node<T> *n = new Node<T>();
            n->data = elem;
            if (pos.n_ptr->prev != nullptr) { // If pos isn't the head.
                pos.n_ptr->prev->next = n;
            } else { // If pos is the head.
                this->head = n;
            }
            n->prev = pos.n_ptr->prev;
            pos.n_ptr->prev = n;
            n->next = pos.n_ptr;
            this->length++;
            return iterator(n);
        }


        /**
         * Inserts a new element before the one stored in the node.
         * @pre node != nullptr and node is in the list.
         * 
         * @param node The node that new node should be inserted infront of.
         * @param elem The element that should be inserted.
         * @return Node* 
         */
         iterator insert(const_iterator pos, T &&elem) {
            Node<T> *n = new Node(std::move(elem));
            if (pos.n_ptr->prev != nullptr) { // If pos isn't the head.
                pos.n_ptr->prev->next = n;
            } else { // If pos is the head.
                this->head = n;
            }
            n->prev = pos.n_ptr->prev;
            pos.n_ptr->prev = n;
            n->next = pos.n_ptr;
            this->length++;
            return iterator(n);
        }


        /**
         * Erase the element. 
         * 
         * @pre *node != nullptr and node is in the list.
         * 
         * @param node 
         */
        void erase(const_iterator pos) {
            assert(pos.n_ptr != this->tail);
            // Correct pointers.
            if (pos.n_ptr->prev != nullptr)
                pos.n_ptr->prev->next = pos.n_ptr->next;
            else 
                this->head = pos.n_ptr->next;
            pos.n_ptr->next->prev = pos.n_ptr->prev;
            this->tail->prev = pos.n_ptr->prev;
            // Delete node
            delete pos.n_ptr;
            // Decremenet size
            this->length--;
        }


        /**
         * The first iterator.
         * 
         * @return iterator The first element.
         */
        iterator begin() {
            return iterator(this->head);
        }


        /**
         * The first const iterator.
         * 
         * @return const_iterator The first element.
         */
        const_iterator begin() const {
            return const_iterator(this->head);
        }


        /**
         * The past the end iterator.
         * 
         * @return iterator The past the end.
         */
        iterator end() {
            return iterator(this->tail);
        }


        /**
         * The past the end const iterator.
         * 
         * @return iterator The past the end.
         */
        const_iterator end() const {
            return const_iterator(this->tail);
        }


        /**
         * The first element.
         * 
         * @return std::string& The first element in the list. 
         */
        T &front() {
            return this->head->data;
        }


        /**
         * The first element.
         * 
         * @return const std::string& The first element in the list.
         */
        const T &front() const {
            return this->head->data;
        }


        /**
         * The last element.
         * 
         * @return std::string& The last element in the list.
         */
        T &back() {
            return this->tail->prev->data;
        }


        /**
         * The last element.
         * 
         * @return const std::string& The last element in the list.
         */
        const T &back() const {
            return this->tail->prev->data;
        }
        

        /**
         * Makes a copy of the other list.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other The other list that a copy should be made of.
         * 
         * @return List& The copy of the other list.
         */
        List<T> &operator=(const List<T> &other) {
            // Deallocate all current nodes in the list.
            this->clear();
            // Copy all the nodes.
            for (List<T>::const_iterator it = other.begin(); it != other.end(); ++it) {
                this->push_back(*it);
            }
            return *this;
        }


        /**
         * Move assignment.
         * 
         * @param other The list to move the data from.
         * @return List<T>& The list the data has been moved to.
         */
        List<T> &operator=(List<T> &&other) {
            this->tail = other.tail;
            this->head = other.head;
            this->length = other.length;
            other.tail = new Node<T>();
            other.head = other.tail;
            other.length = 0;
            return *this;
        }


        /**
         * Compares equality with other list.
         * Computational Complexity: O(n)
         * 
         * @param other The other list that the list should be compared to.
         * @return true if the two list are equal.
         */
        bool operator==(const List<T> &other) const {
            // Check if the length of both list are equal.
            // Otherwise they can't be equal.
            if (this->size() == other.size()) {
                const_iterator thisCurrent = this->begin();
                const_iterator otherCurrent = other.begin();
                // Go through all the elements to check if their data is identical.
                while (thisCurrent != this->end() && *thisCurrent == *otherCurrent) {
                    thisCurrent++;
                    otherCurrent++;
                } 
                return thisCurrent == this->end() && otherCurrent == other.end();
            }
            return false;
        }

        /**
         * Compares inequality with other list. 
         * Computational Complexity: O(n)
         * 
         * @param other The other list that the list should be compared to.
         * @return true if the two list are not equal.
         */
        bool operator!=(const List<T> &other) const {
            return !this->operator==(other);
        }        
    };
}

#endif