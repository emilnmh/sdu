/*
 * Students:
 *      Emiln19 - Emil Nygaard Mathisen Hansen
 *      Jole19 - Johannes Liem Quang Le
 */

import java.util.ArrayList;

public class DictBinTree implements Dict {
    /*
     * Fields
     */
    BinNode root;
    

    /*
     * Constructor. 
     */
    public DictBinTree() {
        root = null;
    }


    /*
     * Inserts key into the tree.
     */
    public void insert(int k) {
        // New node.
        BinNode z = new BinNode(k); 
        
        BinNode y = null; // Current reached node.
        BinNode x = root; // Comparison node. 
        
        // Loop until a leaf is reached.
        while (x != null) { 
            y = x;
            if (z.key() < x.key()) { // Go to left child.
                x = x.left();
            } else { // Go to right child.
                x = x.right();
            }
        }

        // Inserting the node into the tree.
        if (y == null) {
            root = z;
        } else if (z.key() < y.key()) {
            y.setLeft(new BinNode(k));
        } else {
            y.setRight(z);
        }
    }
    

    /*
     * Returns a ordered ArrayList of the keys in the tree.
     * Starts the recursive algorithm.
     */
    public ArrayList<Integer> orderedTraversal() {
        ArrayList<Integer> orderedListOfTree = new ArrayList<>();
        return orderedTraversalAlgorithm(root, orderedListOfTree); 
    }


    private ArrayList<Integer> orderedTraversalAlgorithm(BinNode x, ArrayList<Integer> list) {
        // If not a leaf.
        if (x != null) {
            orderedTraversalAlgorithm(x.left(), list); // Recusive orderedTraverlsalAlgorithm on left child.
            list.add(x.key()); // Add the node to list.
            orderedTraversalAlgorithm(x.right(), list); // Recusive orderedTraverlsalAlgorithm on right child.
        }
        return list;
    }


    /*
     * Returns true if the key is in the tree.
     */
    public boolean search(int k) {
        BinNode x = root;
        // While not a leaf and k is not equal to key of x.
        while (x != null && k != x.key()) { 
            if (k < x.key()) { // Go to left child.
                x = x.left();
            } else { // Go to right child.
                x = x.right();
            }
        }
        return (x != null);
    }


    /*
     * Node of the tree class.
     */
    private class BinNode {
        /*
         * Fields.
         */
        BinNode leftChild;
        BinNode rightChild;
        int key;


        /*
         * Constructor.
         */
        public BinNode(int key) {
            this.key = key;
            leftChild = null;
            rightChild = null;
        }


        /*
         * Getter for leftChild.
         */
        public BinNode left() {
            return leftChild;
        }


        /*
         * Getter for rightChild.
         */
        public BinNode right() {
            return rightChild;
        }


        /*
         * Getter for key. 
         */
        public int key() {
            return key;
        }


        /*
         * Setter for leftChild.
         */
        public void setLeft(BinNode newNode) {
            leftChild = newNode;
        }

        
        /*
         * Setter for rightChild.
         */
        public void setRight(BinNode newNode) {
            rightChild = newNode;
        }
    }
}   