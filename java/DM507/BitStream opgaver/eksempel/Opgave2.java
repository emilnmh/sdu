import java.io.FileInputStream;

public class Opgave2 {
    public static void main(String[] args) throws Exception{
        FileInputStream inFile = new FileInputStream(args[0]);

        BitInputStream in = new BitInputStream(inFile);

        while (inFile.available() > 0) {
            for (int i = 0; i < 8; i++) {
                System.out.print(in.readBit());
            }
            System.out.println();
        }

        in.close();
        inFile.close();
    }
}