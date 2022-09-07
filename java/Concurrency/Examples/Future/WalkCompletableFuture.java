import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

public class WalkCompletableFuture {
    public static void main(String[] args) {
        Map<String, Integer> occurrences = new  ConcurrentHashMap<>();

        // CompletableFuture uses ForkJoinPool.commonPool() as it's default executor.
        try {
            CompletableFuture<Void>[] futures = 
                Files.walk(Paths.get("data"))
                    .filter(Files::isRegularFile) // Filters to only regular files.
                    .map(filePath -> 
                        CompletableFuture.supplyAsync(() -> computeOccurrences(filePath)) // Maps each path to a completableFuture with the task computeOccurrences.
                            .thenAccept(fileOccurrences -> // Runs an extra task right after the task has been completed.
                                fileOccurrences.forEach((word, n) -> occurrences.merge(word, n, Integer::sum)) 
                            )
                    ).collect(Collectors.toList()).toArray(new CompletableFuture[0]); // Converts the the stream of CompletableFuture to an array.
            CompletableFuture.allOf(futures).join(); // Returns a new completableFuture which is completed when all the completable futures has been completed.
        } catch (IOException e) {
            e.printStackTrace();
        }
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