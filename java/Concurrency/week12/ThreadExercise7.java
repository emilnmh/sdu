import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.Arrays;
import java.util.stream.IntStream;

public class ThreadExercise7 {

    public static void main() {

        // Word -> number of times that it appears ovver all files.
        Map< String, Integer > occurrences = new HashMap<>();

        // Filenames
        String[] names = {
            "text1.txt",
			"text2.txt",
			"text3.txt",
			"text4.txt",
			"text5.txt"
        };
        List< String > filenames = Arrays.asList(names);

        CountDownLatch latch = new CountDownLatch( filenames.size() ); // Count down the number of filenames.

        final int[] lWordCount = new int[ filenames.size() ];

        for (int i = 0; i < filenames.size(); i++) {
            final String filename = filenames.get(i);

            final int threadIndex = i;
            new Thread( () -> {
                lWordCount[threadIndex] = computeOccurrences(filename, occurrences);
                latch.countDown();
            } )
            .start();
        }
        
        try {
            latch.await(); // blocks the main method, until latch has counted down.
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        occurrences.forEach( ( word, n ) -> System.out.println(word + ": " + n)); // Prints for each element in occurrences.
        
        System.out.println("Words starting with L: " + IntStream.of(lWordCount).sum());
    }




    private static int computeOccurrences( String filename, Map< String, Integer > occurrences ) {
        final int[] lWordCount = new int[1];
        
        try {
            Files.lines( Paths.get(filename) )
                .flatMap( Words::extractWords )
                .map( String::toLowerCase )
                .forEach( s -> {
                    synchronized( occurrences ) {
                        occurrences.merge( s, 1, Integer::sum );
                    }

                    if ( s.startsWith("l") ) {
                        lWordCount[0]++;
                    }
                }); 
        } catch ( IOException e ) {
            e.printStackTrace();
        }
        return lWordCount[0];
    }
}