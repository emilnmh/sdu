import java.util.NoSuchElementException;

public class BinarySearchTree<E> {
    /*
     * Attributes
     */
    private Node<E> root;


    /*
     * Constructor 
     */
    public BinarySearchTree(int nodePriority, E nodeValue) {
        root = new Node<E>(nodePriority, nodeValue, null);
    }

    /*
     * Creates new node and inserts it into the tree.
     */
    public void insert(int priority, E value) {
        Node<E> newNode = new Node<E>(priority, value, null);
        insert(root, newNode);
    }

    /*
     * Algorithm to insert the node into the tree recursively. 
     */
    private void insert(Node<E> next, Node<E> newNode) {
        if (next.priority() > newNode.priority()) { // If new node has less priority
            if (next.leftNodeIsLeaf()) {
                next.setLeft(newNode);
            } else {
                insert(next.left(), newNode);
            }
        } else { // If new node has greater priority
            if (next.rightNodeIsLeaf()) {
                next.setRight(newNode);
            } else {
                insert(next.right(), newNode);
            }
        }
    }

    public void delete(int priority) {
        delete(root, priority);
    }

    private void delete(Node<E> next, int priority) throws NoSuchElementException {
        if (next.priority() == priority) { // Delete node.
            
        } else if (next.priority() > priority) { // If priority is less than next node's.
            delete(next.left(), priority);
        } else if (next.priority() > priority) { // If priority is greater than next node's.
            delete(next.right(), priority);
        } else { // If no element with priority exists, throw NoSuchElementException.
            throw new NoSuchElementException();
        }
    }

    /*
     * Extracting element
     */
    public void extract() {

    }

    

    /*
     * Node class
     */
    private class Node<E>{
        /*
         * Attributes
         */
        private Node<E> parent; // maybe not necessary.
        private Node<E> leftChild;
        private Node<E> rightChild;
        private E value;
        private int priority;

        public Node(int priority, E value, Node<E> parent) {
            this.value = value;
            this.priority = priority;
            this.parent = parent;
            leftChild = null;
            rightChild = null;
        }

        /*
         * Returns parent.
         */
        public Node<E> parent() {
            return parent;
        }

        /*
         * Returns left child.
         */
        public Node<E> left() {
            return leftChild;
        }

        /*
         * Returns right child.
         */
        public Node<E> right() {
            return rightChild;
        }

        public boolean leftNodeIsLeaf() {
            return (rightChild.equals(null));
        }

        public boolean rightNodeIsLeaf() {
            return (rightChild.equals(null));
        }


        /*
         * Returns successor of node.
         */
        public Node<E> successor() {
            return successor(right());
        }

        /*
         * Returns the successor of node recursively.
         */
        private Node<E> successor(Node<E> next) {
            if (next.leftNodeIsLeaf()) {
                successor(left());
            }
            return next;
        }

        /*
         * Returns precessor of node.
         */
        public Node<E> precessor() {
            return precessor(left());
        }

        /*
         * Returns the precessor of node recursively.
         */
        private Node<E> precessor(Node<E> next) {
            if (!rightNodeIsLeaf()) {
                precessor(next.right());
            } 
            return this;
        }

        /*
         * Set parent of node .
         */
        private void setParent(Node<E> parent) {
            this.parent = parent;
        }


        /*
         * Set left child of node.
         */
        public void setLeft(Node<E> node) {
            leftChild = node;
            leftChild.setParent(this);
        }

        /*
         * Set right child of node.
         */
        public void setRight(Node<E> node) {
            rightChild = node;
            rightChild.setParent(this);
        }

        /*
         * Rotate right
         */
        private void rotateRight(Node<E> node) {

        }

        /*
         * Rotate left
         */
        private void rotateLeft(Node<E> node) {

        }

        /*
         * Returns priority of node.
         */
        public int priority() {
            return priority;
        }

        /*
         * Returns value of node. 
         */
        public E value() {
            return value;
        }
    }
}