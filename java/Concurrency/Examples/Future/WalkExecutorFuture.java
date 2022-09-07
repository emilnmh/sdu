import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

public class WalkExecutorFuture {
    public static void main(String[] args) {
        Map<String, Integer> occurrences = new HashMap<>();
        ExecutorService executor = Executors.newWorkStealingPool(); // Use this by default. Tries to create an optimal executor. Stealing work from other threads if they haven't been started yet.

        try {
            List<Future<Map<String, Integer>>> futures = 
                Files.walk(Paths.get("data")) // Walks through all files the the directory "data".
                    .filter(Files::isRegularFile) // Only uses regular files
                    .map(filepath -> 
                        executor.submit(() -> computeOccurrences(filepath))) // For each regular file, submit a executor that runs the computeOccurrences on the file.
                    .collect(Collectors.toList()); // Takes all the elements of a stream and collects them.
            // Problem: take each future in order, so they will have to wait for the one before to be finished.
            for (Future<Map<String, Integer>> future : futures) { // Iteration over the list of futures.
                Map<String, Integer> fileOccurrences = future.get(); // When the result is eventually computed, set fileOccurrences to it.
                fileOccurrences.forEach((word, n) -> occurrences.merge(word, n, Integer::sum)); // Merging the result with the global occurrences.
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