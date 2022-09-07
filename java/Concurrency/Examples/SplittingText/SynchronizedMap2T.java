import java.util.Map;
import java.io.IOException;
import java.util.HashMap;
import java.nio.file.Files;
import java.nio.file.Paths;

public class SynchronizedMap2T {
    public static void main() {

        // Word -> number of times that it appears ovver all files.
        Map< String, Integer > occurrences = new HashMap<>();
        
        // Thread t1
        Thread t1 = new Thread( () -> {
            computeOccurrences("text1.txt", occurrences); // Computes every occurrences of the words in text1.txt.
        } );

        // Thread t2
        Thread t2 = new Thread( () -> {
            computeOccurrences("text2.txt", occurrences); // Computes every occurrences of the words in text2.txt.
        } );

        t1.start();
        t2.start();

        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        occurrences.forEach( ( word, n ) -> System.out.println(word + ": " + n)); // Prints for each element in occurrences.
    }

    private static void computeOccurrences( String filename, Map< String, Integer > occurrences ) {
        try {
            Files.lines( Paths.get(filename) )
                .flatMap( Words::extractWords )
                .map( String::toLowerCase )
                .forEach( s -> {
                    synchronized( occurrences ) {
                        occurrences.merge( s, 1, Integer::sum );
                    }
                }); 
        } catch ( IOException e ) {
            e.printStackTrace();
        }
    }
}