import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.io.IOException;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.stream.IntStream;
import java.util.Map;
import java.util.Optional;


public class ThreadExercise12 {
    public static void main() {
        Map< String, Integer > occurrences = new ConcurrentHashMap< String, Integer>();

        int maxThreads = Runtime.getRuntime().availableProcessors() - 1;
        CountDownLatch latch = new CountDownLatch(maxThreads);
        final BlockingDeque<Optional<Path>> tasks = new LinkedBlockingDeque<>(); // Like a queue where you can add/remove from front and end.

        IntStream.range(0, maxThreads).forEach(i -> {
            new Thread(() -> {
                try {
                    Optional<Path> task;
                    do {
                        task = tasks.take(); // Removes and returns head of the deque.
                        task.ifPresent(filepath -> computeOccurrences(filepath, occurrences)); // If it is present, computesOccurrences.
                    } while (task.isPresent()); // Loop until there isn't a present element in the deque.
                    tasks.add(task); // Adding the task back to tasks.
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                latch.countDown();
            }).start();
        });

        try {
            Files.walk(Paths.get("data")) 
                .filter(file -> file.toString().endsWith(".txt")) // Filters to only files ending with ".txt"-
                .forEach(path -> tasks.add(Optional.of(path))); // Adds the optional path to tasks.
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        tasks.add(Optional.empty()); // Adds empty optional at the end. Used to know when the we're done.
        
        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        occurrences.forEach((k, v) -> {
            System.out.println(k + ": " + v);
        });
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