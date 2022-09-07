import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;

public class StreamExecise {

    public static void main(String[] arg) {
        try {
            //execise1();
            //execise2();
            //execise3();
            //execise4();
            execise5();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }  

    /*
     * Execise 1
     *
	 * - Create a file with many (>100) lines of text.
	 * For example, you can use this website: loremipsum.io
	 * - Use Files.lines to get a stream of the lines contained within the file.
	 * - Use Stream::filter and Stream::forEach to print on screen each line that ends with a dot.
     */
    public static void execise1() throws IOException {
        Files.lines(Paths.get("randomSentences.txt"))
            .filter(l -> l.endsWith("."))
            .forEach(System.out::println);
    }

    /*
     * Execise 2
     *
	 * - Create a stream of lines for the file created in StreamExercise1.
	 * - Use Stream::filter and Stream::collector (the one with three parameters)
	 * to create an ArrayList of all lines that start with a "C".
	 * - Suggestion: look at https://docs.oracle.com/javase/8/docs/api/java/util/stream/Stream.html#collect-java.util.function.Supplier-java.util.function.BiConsumer-java.util.function.BiConsumer-
     */
    public static void execise2() throws IOException {

        ArrayList<String> listWithC = new ArrayList<>();

        listWithC = Files.lines(Paths.get("randomSentences.txt"))
            .filter((s) -> s.startsWith("C"))
            .collect(ArrayList::new, ArrayList::add, ArrayList::addAll);
    
        for (String s : listWithC) {
            System.out.println(s);
        }
    }


    /*
     * Execise 3
     * 
	 * - Create a stream of lines for the file created in StreamExercise1.
	 * - Use Stream::filter and Stream::count to count how many lines
	 * contain the letter "L".
	*/ 
    public static void execise3() throws IOException {
        System.out.println(
            Files.lines(Paths.get("randomSentences.txt"))
                .filter((s) -> s.contains("l"))
                .count()
        );
    }
    
    /*
     * Execise 4
     *
	 * - Create a stream of lines for the file created in StreamExercise1.
	 * - Use Stream::mapToInt and IntStream::sum to count how many times
	 * the letter "C" occurs in the entire file.
	 */
    public static void execise4() throws IOException {
        System.out.println(
            Files.lines(Paths.get("randomSentences.txt"))
                .mapToInt((l) -> (int)l.chars().filter((c) -> c == 'C').count())
                .sum()
        );
    }

    /* 
     * Execise 5 
     *
     * ! (Exercises marked with ! are more difficult.)
	 * 
	 * - Create a stream of lines for the file created in StreamExercise1.
	 * - Use Stream::map to map each line to a HashMap<String, Integer> that
	 * stores how many times each character appears in the line.
	 * For example, for the line "abbc" you would produce a map with entries:
	 *   a -> 1
	 *   b -> 2
	 *   c -> 1
	 * - Use Stream::reduce(T identity, BinaryOperator<T> accumulator)
	 * to produce a single HashMap<String, Integer> that stores
	 * the results for the entire file.
	 */

     public static void execise5() throws IOException {
        HashMap<String, Integer> characterCount = new HashMap<>();

        characterCount = Files.lines(Paths.get("randomSentences.txt"))
            .map((l) -> { // Maps each line to a hashmap.

                // Makes a new hash map
                HashMap<String, Integer> res = new HashMap<>();

                // For each line.
                // Merge hashmap, by counting one up each time a character appears. 
                l.chars().forEach( (c) -> res.merge(String.valueOf((char) c), 1, (k, v) -> k+v));

                return res;
            })
            .reduce(new HashMap<String, Integer>(), (prev, cur) -> {
                cur.forEach((k, v) -> prev.merge(k, v, (a, b) -> a+b));
                return prev;
            });

        System.out.println(characterCount);
     }
}