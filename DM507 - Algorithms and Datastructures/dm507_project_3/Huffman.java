
public class Huffman {
    /**
     * Generates a huffman tree structure.
     * 
     * @param C the frequency of all the characters, with the ascii byte to int as index.
     * @return the root node of the tree, with all nodes having 0 or 2 nodes as children.
     */
    public static Element tree(int[] C) {
        int n = C.length;
        // Creates a heap from C.
        PQHeap Q = new PQHeap();  
        for (int i = 0; i < n; i++) {
            // Values of C are the keys and the indexes are values.
            Q.insert(new Element(C[i], new TreeNode(i)));
        }

        for (int i = 0; i < n - 1; i++) {
            // Extracting the two minimums in the heap.
            Element x = Q.extractMin();
            Element y = Q.extractMin();

            // Creating a new node, with the two extracted nodes as children, and inserting it into the heap.
            TreeNode tree = new TreeNode(x, y);
            Element z = new Element(x.getKey() + y.getKey(), tree);
            Q.insert(z);
        }

        // Returns root.
        return Q.extractMin(); 
    }
}