public class Main {
    public static void main(String[] args) {
        DictBinTree tree = new DictBinTree();
        tree.insert(10);
        tree.insert(6);
        tree.insert(2);
        tree.insert(13);
        tree.insert(5);
        tree.insert(11);
        tree.insert(19);

        System.out.println("Does key 13 exists in tree: " + tree.search(13));
        System.out.println("Does key 12 exists in tree: " + tree.search(12));
        System.out.println("Does key 10 exists in tree: " + tree.search(10));
        System.out.println("Does key 5 exists in tree: " + tree.search(5));

        System.out.println(tree.orderedTraversal());
    }
}