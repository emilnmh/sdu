#ifndef LIST_H
#define LIST_H

#include <string>

namespace DM852 {
    namespace LIST_NODE {
        class Node {
        public:
            std::string data;
            Node *next;
            Node *prev;
        };
    }

    class List {
    private:
        LIST_NODE::Node *head = nullptr;
        LIST_NODE::Node *tail = nullptr;
        int length = 0;
    public:
        /**
         * Construct a new List object.
         */
        List();


        /**
         * Copy constructor.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other The other list to copy the elements from.
         */
        List(const List &other);


        /**
         * Destroy the List object.
         * Computational Complexity: O(n), n = size().
         */
        ~List();

        /**
         * Return the number of elements stored in the list.
         * 
         * @return int Number of elements stored in the list.
         */
        int size() const;

        /**
         * Returns true if the list is empty.
         * 
         * @return true | false
         */
        bool empty() const;

        /**
         * Inserts a new element in the end.
         * 
         * @param elem The element to be inserted.
         */
        void push_back(const std::string &elem);

        /**
         * Inserts a new element before the one stored in the node.
         * @pre node != nullptr and node is in the list.
         * 
         * @param node The node that new node should be inserted infront of.
         * @param elem The element that should be inserted.
         * @return LIST_NODE::Node* 
         */
        LIST_NODE::Node *insert(LIST_NODE::Node *node, const std::string &elem);
        
        /**
         * Erase all elements.
         * Computational Complexity: O(n), n = size().
         */
        void clear();
        
        /**
         * Erase the last element.
         */
        void pop_back();

        /**
         * Erase the element. 
         * 
         * @pre *node != nullptr and node is in the list.
         * 
         * @param node 
         */
        void erase(LIST_NODE::Node *node);

        /**
         * The first element.
         * 
         * @return std::string& The first element in the list. 
         */
        std::string &front();

        /**
         * The first element.
         * 
         * @return const std::string& The first element in the list.
         */
        const std::string &front() const;

        /**
         * The last element
         * 
         * @return std::string& The last element in the list.
         */
        std::string &back();

        /**
         * The last element
         * 
         * @return const std::string& The last element in the list.
         */
        const std::string &back() const;

        /**
         * The first node.
         * 
         * @return LIST_NODE::Node* The first node in the list.
         */
        LIST_NODE::Node *begin();

        /**
         * The first node.
         * 
         * @return const LIST_NODE::Node* The first node in the list.
         */
        const LIST_NODE::Node *begin() const;

        /**
         * Return nullptr.
         * 
         * @return LIST_NODE::Node* nullptr
         */
        LIST_NODE::Node *end();

        /**
         * Return nullptr.
         * 
         * @return const LIST_NODE::Node* nullptr
         */
        const LIST_NODE::Node *end() const;

        /**
         * Makes a copy of the other list.
         * Computational Complexity: O(n), n = size().
         * 
         * @param other The other list that a copy should be made of.
         * 
         * @return List& The copy of the other list.
         */
        List &operator=(const List &other);

        /**
         * Compares equality with other list.
         * Computational Complexity: O(n)
         * 
         * @param other The other list that the list should be compared to.
         * @return true if the two list are equal.
         */
        bool operator==(const List &other) const;
        
        /**
         * Compares inequality with other list. 
         * Computational Complexity: O(n)
         * 
         * @param other The other list that the list should be compared to.
         * @return true if the two list are not equal.
         */
        bool operator!=(const List &other) const;
    };
}

#endif