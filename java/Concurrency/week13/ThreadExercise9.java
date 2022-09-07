import java.util.Map;
import java.util.Set;
import java.util.HashSet;
import java.util.List;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.stream.*;
import java.util.concurrent.ConcurrentHashMap;

public class ThreadExercise9 {
    public static void main() {
        Map<Character, Set<String>> mappedWords = new ConcurrentHashMap<>();
        List<Thread> threads = new ArrayList<>();

        try {
            Files.walk(Paths.get("data"))
                .filter(Files::isRegularFile)
                .map(path -> new Thread(() -> mapWords(path, mappedWords)))
                .forEach(thread -> threads.add(thread));
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

        // Printing all the keys with the words mapped to it.
        mappedWords.forEach((k, v) -> {
            System.out.println(k + ": " + v);
        });
    }


    // Mapping all words to its first character.
    private static void mapWords(Path path, Map<Character, Set<String>> mappedWords) {
        try {
            Files.lines(path) // Reads all lines from file, and puts it into a Stream<String>
                .flatMap( line -> extractWords(line) )
                .forEach( word -> {
                    mappedWords.merge(word.charAt(0), new HashSet<>(Arrays.asList(word)), (v, w) -> {
                        v.add(word);
                        return v;
                    });
                });
        } catch (IOException e) {
            e.printStackTrace();
        }
    }




    // Extracting words from a line, and returns it as a Stream<string>
    public static Stream<String> extractWords(String line) {
        List<String> words = new ArrayList<>();

        BreakIterator iterator = BreakIterator.getWordInstance();
        iterator.setText(line);

        int start = iterator.first();
        int end = iterator.next();

        while(end != BreakIterator.DONE) { // Iterate over the whole boundary.
            String word = line.substring( start, end );
            if ( Character.isLetterOrDigit( word.charAt( 0 ) ) ) { // Checks if it is a seperater (Like space).
                words.add( word );
            }
            start = end;
            end = iterator.next();
        }
        return words.stream();
    }
}