import java.io.FileInputStream;
import java.io.IOException;

public class Opgave3 {
    public static void main(String[] args) throws IOException {
        FileInputStream inFile = new FileInputStream(args[0]);

        BitInputStream in = new BitInputStream(inFile);

        for (int i = 0; i < 4; i++) {
            System.out.println(in.readInt());
        }

        in.close();
        inFile.close();
    }
}