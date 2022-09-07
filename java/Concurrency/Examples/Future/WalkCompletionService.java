import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class WalkCompletionService {
    public static void main(String[] args) {
        Map<String, Integer> occurrences = new HashMap<>();
        ExecutorService executor = Executors.newWorkStealingPool(); // Use this by default. Tries to create an optimal executor. Stealing work from other threads if they haven't been started yet.
        ExecutorCompletionService<Map<String, Integer>> completionService = new ExecutorCompletionService<>(executor); // Helps you with knowing when a future is completed.

        try {
            long pendingTasks = 
                Files.walk(Paths.get("data")) // Walks through all files the the directory "data".
                    .filter(Files::isRegularFile) // Only uses regular files
                    .map(filepath -> 
                        completionService.submit(() -> computeOccurrences(filepath))) // For each regular file, the completionService submits its task to the executor.
                    .count(); // Counts the number of elements in the stream.
            while (pendingTasks > 0) { // While there's still pending tasks.
                Map<String, Integer> fileOccurrences = completionService.take().get(); // Sets fileoccurrences to a future from completionService that has been completed. Waits until any task have been completed.
                fileOccurrences.forEach((word, n) -> occurrences.merge(word, n, Integer::sum)); // Merging the result with the global occurrences.
                pendingTasks--; // Decreasing pendingTasks by 1.
            }
        } catch (InterruptedException | ExecutionException | IOException e) {
            e.printStackTrace();
        }

        try {
            executor.shutdown();
            executor.awaitTermination(1, TimeUnit.DAYS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        occurrences.forEach((k, v) -> System.out.println(k + ": " + v));
    }

    // Computes the occurrences of each word in a file.
    public static Map<String, Integer> computeOccurrences(Path textFile) {
        Map<String, Integer> occurrences = new HashMap<>();
        try {
            Files.lines(textFile)
                .flatMap(Words::extractWords)
                .map(String::toLowerCase)
                .forEach(s -> occurrences.merge(s, 1, Integer::sum));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return occurrences;
    }
}   