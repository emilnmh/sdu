import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

public class WalkExecutor {
    public static void main() {
        
        Map< String, Integer > occurrences = new ConcurrentHashMap< String, Integer>();
        //ExecutorService executor = Executors.newCachedThreadPool(); // An executor that manages threads.
        ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors()); // An executor that manages threads with a maximum of threads running at the same time.
        try {
            Files.walk( Paths.get( "data" ) )
                .filter(Files::isRegularFile) 
                .forEach( filepath -> {
                    executor.submit(() -> { // Submits a thread to the task.
                        computeOccurrences(filepath, occurrences);
                    });
                }); 
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        try {
            executor.shutdown(); // Disallows the executor accept new tasks.
            executor.awaitTermination(1, TimeUnit.DAYS); // Waits for one day for all tasks to terminate.
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
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