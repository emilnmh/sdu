class TreeNode {
    private int value;
    private Element left;
    private Element right;

    /**
     * Constructor
     * 
     * @param value the integer value of the node.
     */
    public TreeNode(int value) {
        this.value = value;
        left = null;
        right = null;
    }

    /**
     * Constructor
     * 
     * @param left the left child of the node
     * @param right the right chold of the node
     */
    public TreeNode(Element left, Element right) {
        this.left = left;
        this.right = right;
    }

    /**
     * 
     * @return the integer value of the node
     */
    public int value() {
        return value;
    }

    /**
     * 
     * @return the left children of the node
     */
    public Element left() {
        return left;
    }

    /**
     * 
     * @return the right children of the node
     */
    public Element right() {
        return right;
    }

    /**
     * 
     * @return true if the node is a leaf.
     */
    public boolean isLeaf() {
        return (right == null);
    }
}