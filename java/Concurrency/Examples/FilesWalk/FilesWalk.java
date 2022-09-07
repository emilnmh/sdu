import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

public class FilesWalk {
    public static void main() {
        
        Map< String, Integer > occurrences = new ConcurrentHashMap< String, Integer>();
        List< Thread > threads = new ArrayList<>();

        try {
            Files.walk( Paths.get( "data" ) ) // Returns a stream of paths, everything inside the root directory. 
                .filter( Files::isRegularFile ) // Filters the stream of paths, to only the files.
                .map( path -> new Thread( () -> computeOccurrences( path, occurrences )) )
                .forEach( thread -> threads.add( thread ) ); // Adding all the threads to the list.
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        // Starting all the threads
        threads.forEach( Thread::start );

        // joining all the threads
        threads.forEach( thread -> {
            try {
                thread.join();
            }  catch (InterruptedException e) {
                e.printStackTrace();
            }
        });

        // Printing all the occurrences.
        occurrences.forEach( ( word, n ) -> System.out.println( word + ": " + n ));
        
    }

    private static void computeOccurrences( Path path, Map< String, Integer > occurrences ) {
        try {
            Files.lines( path )
                .flatMap( Words::extractWords )
                .map( String::toLowerCase )
                .forEach( s -> occurrences.merge( s, 1, Integer::sum ) ); 
        } catch ( IOException e ) {
            e.printStackTrace();
        }
    }
}