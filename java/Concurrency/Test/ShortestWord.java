import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.stream.Collectors;

public class ShortestWord {
    public static void main(String[] args) {
        System.out.println(shortestWord());
    }

    private static String shortestWord() {
        String[] shortestWord = new String[1];

        try {
            CompletableFuture<Void>[] futures = 
                Files.walk(Paths.get("data"))
                    .filter(Files::isRegularFile)
                    .map(filePath -> 
                        CompletableFuture.supplyAsync(() -> shortestWordInFile(filePath)
                        ).thenAccept(shortestWordInFile -> {
                            synchronized (shortestWord) {
                                if (shortestWord[0] == null || shortestWord[0].length() > shortestWordInFile.length() || shortestWord[0].compareTo(shortestWordInFile) > 0) {
                                    shortestWord[0] = shortestWordInFile;
                                }
                            }
                        })
                    ).collect(Collectors.toList()).toArray(new CompletableFuture[0]);
            CompletableFuture.allOf(futures).join();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return shortestWord[0];
    }

    private static String shortestWordInFile(Path filePath) {
        final String[] shortestWordInFile = new String[1];
        try {
            Files.lines(filePath)
                .flatMap(line -> {
                    List<String> words = new ArrayList<>(); // List of all the words in the line.

                    BreakIterator breakIterator = BreakIterator.getWordInstance(); // BreakIterator that breaks at every word.
                    breakIterator.setText(line);

                    int start = breakIterator.first(); // Starting boundary of word
                    int end = breakIterator.next(); // Ending boundary of word
                    while (end != BreakIterator.DONE) { // Iterates until the BreakIterator has reached the end of the line.
                        String word = line.substring(start, end);
                        // Checks if the word isn't a seperator
                        if (Character.isLetterOrDigit(word.charAt(0))) {
                            words.add(word);
                        }
                        // Moves to the next word in the line.
                        start = end;
                        end = breakIterator.next();
                    }

                    return words.stream();
                })
                .forEach(word -> {
                    if (shortestWordInFile[0] == null || word.length() < shortestWordInFile[0].length() || word.length() == word.length() && word.compareTo(shortestWordInFile[0]) < 0) {
                        shortestWordInFile[0] = word;
                    }
                });
        } catch (IOException e) {
            e.printStackTrace();
        }

        return shortestWordInFile[0];
    }
}

/*
    AtomicReference<String> shortestWord = new AtomicReference<>();

    try {
        CompletableFuture<Void>[] futures = 
            Files.walk(Paths.get("data"))
                .filter(path -> path.toString().endsWith(".txt"))
                .map(filePath ->
                    CompletableFuture.supplyAsync(() -> {
                        // Find the shortest word in file.
                        final String[] shortestWordInFile = new String[0];

                        try {
                            Files.lines(filePath)
                            .flatMap(line -> {
                                List<String> words = new ArrayList<>(); // List of all the words in the line.

                                BreakIterator breakIterator = BreakIterator.getWordInstance(); // BreakIterator that breaks at every word.
                                breakIterator.setText(line);

                                int start = breakIterator.first(); // Starting boundary of word
                                int end = breakIterator.next(); // Ending boundary of word
                                while (end != BreakIterator.DONE) { // Iterates until the BreakIterator has reached the end of the line.
                                    String word = line.substring(start, end);
                                    // Checks if the word isn't a seperator
                                    if (Character.isLetterOrDigit(word.charAt(0))) {
                                        words.add(word);
                                    }
                                    // Moves to the next word in the line.
                                    start = end;
                                    end = breakIterator.next();
                                }

                                return words.stream();
                            })
                            .forEach(word -> {
                                if (shortestWordInFile[0].isEmpty() || word.length() < shortestWordInFile[0].length() || word.length() == word.length() && word.compareTo(shortestWordInFile[0]) < 0) {
                                    shortestWordInFile[0] = word;
                                }
                            });
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        return shortestWordInFile;
                    }).thenAccept(shortestWordInFile -> {
                        // Compare the shortest word in the file with the shortest word found.
                        shortestWord.compareAndSet(null, shortestWordInFile[0]);

                    })
                ).collect(Collectors.toList()).toArray(new CompletableFuture[0]);
        CompletableFuture.allOf(futures).join();
    } catch (IOException e) {
        e.printStackTrace();
    }
    
    return shortestWord.get(); 
 */