import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.Map;

// Runs until the word nulla is found.
public class ThreadExercise13 {

    public static void main() {

        AtomicBoolean nullaFound = new AtomicBoolean(false);
        
        Map< String, Integer > occurrences = new ConcurrentHashMap< String, Integer>();
        ExecutorService executor = Executors.newCachedThreadPool(); // An executor that manages threads.
        //ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors()); // An executor that manages threads with a maximum of threads running at the same time.
        
        try {
            Files.walk( Paths.get( "data" ) )
                .filter(Files::isRegularFile) 
                .forEach( filepath -> {
                    executor.submit(() -> { // Submits a thread to the task.
                        computeOccurrences(filepath, occurrences, nullaFound);
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

        occurrences.forEach((k, v) -> {
            System.out.println(k + ": " + v);
        });
    }

    private static void computeOccurrences( Path path, Map< String, Integer > occurrences, AtomicBoolean nullaFound) {
        try {
            Files.lines( path )
                .flatMap( Words::extractWords )
                .map( String::toLowerCase )
                .forEach( s -> {
                    if (!nullaFound.get()) {
                        if (s.equals("nulla"))
                            nullaFound.set(true);
                        occurrences.merge(s, 1, (v1, v2) -> v1 + v2);
                    }
                }); 
        } catch ( IOException e ) {
            e.printStackTrace();
        }
    }
}