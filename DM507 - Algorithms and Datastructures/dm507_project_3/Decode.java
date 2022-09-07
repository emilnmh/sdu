import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class Decode {
    public static void main(String[] args) {
        BitInputStream inputStream;
        FileOutputStream outputStream;
        try {
            int[] occurrences = new int[256];
            // Reads 256 ints from file
            inputStream = new BitInputStream(new FileInputStream(args[0]));
            for (int i = 0; i < 256; i++) {
                occurrences[i] = inputStream.readInt();
            }
           

            // Create huffman tree structure.
            Element root = Huffman.tree(occurrences);

            int bit;
            Element pointer = root;
            outputStream = new FileOutputStream(args[1]);

            while ((bit = inputStream.readBit()) != -1) {
                TreeNode node = (TreeNode)pointer.getData();

                if (bit == 0) { // Go left
                    pointer = node.left();
                } else if (bit == 1) { // Go right
                    pointer = node.right();
                }

                // If the node is a leaf
                if (((TreeNode)pointer.getData()).isLeaf()) {
                    // Write character to file.
                    int asciiByte = ((TreeNode)pointer.getData()).value();
                    outputStream.write(asciiByte);
                    // Move pointer to root.
                    pointer = root;
                }
            }
            
            inputStream.close();
            outputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        
    }
}