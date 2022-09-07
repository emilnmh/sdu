import java.util.ArrayList;
import java.util.Scanner;

public class Treesort {
    public static void main(String[] args) {
        DictBinTree tree = new DictBinTree();

        Scanner sc = new Scanner(System.in);
        while (sc.hasNextInt()) {
            tree.insert(sc.nextInt());
        }

        ArrayList<Integer> orderedList = tree.orderedTraversal();
        for (int i : orderedList) {
            System.out.println(i);
        }
    }
}