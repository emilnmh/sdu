import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Stream;

public class findWords {

	public static void main() {
		List<WordLocation> words = findWord(Paths.get("data"), "lorem", 100000);
	}


	private static List<WordLocation> findWord(Path dir, String word, int limit) {
		List<WordLocation> wordLocations = new ArrayList<>();
		
		AtomicInteger wordsAdded = new AtomicInteger(0);
		AtomicBoolean cancelled = new AtomicBoolean(false);

		ExecutorService executor = Executors.newWorkStealingPool();

		try {
			Files.walk(dir)
				.filter(path -> path.toString().endsWith(".txt"))
				.flatMap(path -> {
					try {
						int[] lineCounter = {0};
						return Files.lines(path)
							.map(line -> new LineInfo(++lineCounter[0], line, path));
					} catch (IOException e) {
						return Stream.empty();
					}
				})
				.forEach(line -> 
					executor.submit(() -> extractWords(line, word, wordLocations, wordsAdded, limit, cancelled))
				);
			
			executor.shutdown();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (CancellationException e) {
			System.err.println(e);
		}

		return wordLocations;
	}





    
    public static void extractWords(LineInfo lineInfo, String word, List<WordLocation> wordLocations, AtomicInteger wordsAdded, int limit, AtomicBoolean cancelled) {
		String line = lineInfo.getWords();

		BreakIterator breakIterator = BreakIterator.getWordInstance(); // BreakIterator that breaks at every word.
		breakIterator.setText(line);

		int start = breakIterator.first(); // Starting boundary of word
		int end = breakIterator.next(); // Ending boundary of word
		while (end != BreakIterator.DONE) { // Iterates until the BreakIterator has reached the end of the line.
			if (cancelled.get())
				throw new CancellationException();
			
			String nextWord = line.substring(start, end);
			

			// Check if it is a seperator.
			if (Character.isLetterOrDigit(nextWord.charAt(0))) {
				if (word.equals(nextWord)) {
					synchronized (wordLocations) {
						if (!cancelled.get()) {
							wordLocations.add(new WordLocation(lineInfo.getPath(), lineInfo.getLineNumber()));

							if (wordsAdded.incrementAndGet() == limit) {
								cancelled.set(true);
								throw new CancellationException();
							}
						}
					}
				}
			}
			// Moves to the next word in the line.
			start = end;
			end = breakIterator.next();
		}
    }
    


	private static class LineInfo {
		private int lineNumber;
		private String words;
		private Path path;

		private LineInfo(int lineNumber, String words, Path path) {
			this.lineNumber = lineNumber;
			this.words = words;
			this.path = path;
		}

		public String getWords() {
			return words;
		}

		public int getLineNumber() {
			return lineNumber;
		}

		public Path getPath() {
			return path;
		}
	}


    private static class WordLocation {
		private final Path filepath; // the file where the word has been found
		private final int line; // the line number at which the word has been found

		private WordLocation(Path filepath, int line) {
			this.filepath = filepath;
			this.line = line;
		}

		public String toString() {
			return "Path: " + filepath.toString() + " line: " + line;
		}
    }
}