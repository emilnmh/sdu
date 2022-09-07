import java.io.FileInputStream;
import java.util.HashMap;
import java.util.Map;

public class Opgave1 {
    // 1) Læser en fil en byte ad gangen.
    // 2) Tælle hvor mange 256 mulige bytes filen indeholder.
    // 3) Til sidst udskrive en tabel i stil med:
    // Byte 0: 0
    // Byte 1: 4
    // Byte 2: 1
    
    public static void main(String[] args) throws Exception {
        FileInputStream inFile = new FileInputStream(args[0]);
        Map<Integer, Integer> byteCounterMap = new HashMap<>();
        for (int i = 0; i < 255; i++) {
            byteCounterMap.put(i, 0);
        }

        while (inFile.available() > 0) {
            int byteRead = inFile.read();
            byteCounterMap.merge(byteRead, 1, Integer::sum);
        }
        
        byteCounterMap.forEach((k, v) -> System.out.println("Byte " + k + ": " + v));

        inFile.close();
    }
}