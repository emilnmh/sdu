import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;


public class Encode {
    public static void main(String[] args) {
        FileInputStream inputStream;
        BitOutputStream outputStream;
        try {
            // Goes through the file and count how many times a byte occur.
            int[] occurrences = new int[256];
            inputStream = new FileInputStream(args[0]);

            while (inputStream.available() > 0) {
                occurrences[inputStream.read()] += 1;
            }
            inputStream.close();


            // Creates the huffman tree structure.
            Element tree = Huffman.tree(occurrences);

            // All the codes for the different characters.
            String[] codes = codesInorderTraversel(tree);

            // Writting all the codes for the output file.
            outputStream = new BitOutputStream(new FileOutputStream(args[1]));
            for (int occurrence : occurrences) {
                outputStream.writeInt(occurrence);
            }
            

            // Going through all the bytes in input file, getting their codes, and writing all the codes to the output file.
            inputStream = new FileInputStream(args[0]);
            while (inputStream.available() > 0) {
                String code = codes[inputStream.read()];
                for (int i = 0; i < code.length(); i++) {
                    int bit = Integer.parseInt(code.substring(i, i+1));
                    outputStream.writeBit(bit);
                }
            }
            inputStream.close();
            outputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Starts the recursive function inorderTraversal.
     * 
     * @param root the root node of the huffman tree.
     * @return an Array string of all the codes for the 256 different characters.
     */
    private static String[] codesInorderTraversel(Element root) {
        return inorderTraversal(root, new String[256], "");
    }

    /**
     * Generetes all the codes for all the characters recursively, by going down the tree.
     * 
     * @param pointer is the pointer to the current node in the huffman tree.
     * @param codes contains all the different codes.
     * @param code the current code that is currently being genereted.
     * @return all the codes.
     */
    private static String[] inorderTraversal(Element pointer, String[] codes, String code) {
        TreeNode node = (TreeNode)pointer.getData();
        
        if (!node.isLeaf()) {
            inorderTraversal(node.left(), codes, code + "0"); // Going left in the tree, adds a 0 to the code.
            inorderTraversal(node.right(), codes, code + "1"); // Going right in the tree, adds a 1 to the code.
        } else {
            int asciiByte = node.value();
            // Inserts the code of the character, on the index of the ascii value.
            codes[asciiByte] = code;
        }

        return codes;
    }
}