import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Exam {
    public static void main(String[] args) {
		System.out.println(longestWord(Paths.get("data")));
		//wordStartingWith(Paths.get("data"), "lor", false);
    }


	private static String longestWord(Path dir) {
		AtomicReference<String> longestWord = new AtomicReference<>("");
		
		try {
			List<CompletableFuture<Void>> futures = Files.walk(dir)
				.filter(path -> path.toString().endsWith(".txt"))
				.collect(Collectors.toList())
				.parallelStream() // Is this needed?
				.map(filePath -> 
					CompletableFuture.supplyAsync(() -> {
						String[] longestWordInFile = {""};
						try {
							Files.lines(filePath)
								.parallel() // Is this needed?
								.flatMap(line -> extractWords(line))
								.forEach(word -> {
									if (shortestWord(word, longestWordInFile[0]) == longestWordInFile[0]) {
										longestWordInFile[0] = word;
									}
								});
						} catch (IOException e) {
							e.printStackTrace();
						}
						
						return longestWordInFile[0];
					})
						.thenAccept(longestWordInFile -> {
							longestWord.compareAndSet(shortestWord(longestWordInFile, longestWord.get()), longestWordInFile);
						})
				).collect(Collectors.toList());
				

			CompletableFuture
				.allOf(futures.toArray(new CompletableFuture[0]))
				.join();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return longestWord.get();
	}


	


    /**
     * Optional<LocatedWord>
     */
    public static void wordStartingWith(Path dir, String prefix, boolean caseSensitive) {
		try {
			Files.walk(dir)
				.filter(path -> path.toString().endsWith(".txt"))
				.collect(Collectors.toList())
				.parallelStream()
				.flatMap(file -> {
					try {
						return Files.lines(file);
					} catch (IOException e) {
						return Stream.empty();
					}
				})
				.flatMap(line -> extractWords(line))
				.forEach(System.out::println);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	

	private static String shortestWord(String w1, String w2) {
		return (w1.length() > w2.length() || w1.length() == w2.length() && w1.compareTo(w2) > 0 ? w2 : w1);
	}

    
    public static Stream<String> extractWords(String line) {
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
	}

	
}

