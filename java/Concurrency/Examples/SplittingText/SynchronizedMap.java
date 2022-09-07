import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.Arrays;

public class SynchronizedMap {
    public static void main() {

        // Word -> number of times that it appears ovver all files.
        Map< String, Integer > occurrences = new HashMap<>();

        String[] names = {
            "text1.txt",
			"text2.txt",
			"text3.txt",
			"text4.txt",
			"text5.txt"
        };
        List< String > filenames = Arrays.asList(names);

        CountDownLatch latch = new CountDownLatch( filenames.size() ); // Count down the number of filenames.

        // Creates a new thread for each file and starts it.
        filenames.stream()
            .map( filename -> new Thread( () -> {
                computeOccurrences( filename, occurrences );
                latch.countDown(); // Decrement the number in latch by 1.
            } ) )
            .forEach( Thread::start );
        
        try {
            latch.await(); // blocks the main method, until latch has counted down.
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