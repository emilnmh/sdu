/**
 * Emil Nygaard Mathisen Hansen - emiln19
 */

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/*
This is the exam for DM563 - Concurrent Programming, Spring 2020.

Your task is to implement the following methods of class Exam:
- shortestWord;
- longestWord;
- wordStartingWith;
- findWord.

These methods search text files for particular words.
You must use a BreakIterator to identify words in a text file,
which you can obtain by calling BreakIterator.getWordInstance().
The usage of BreakIterator is explained in the video lecture 02-BreakIterator.

The implementations of these methods must exploit concurrency to achieve improved performance.

The only code that you can change is the implementation of these methods.
In particular, you cannot change the signatures (return type, name, parameters) of any method, and you cannot edit method main.
The current code of these methods throws an exception: remove that line before proceeding on to the implementation.

You can find a general explanation in the video lecture 10-Exam, and at the following webpage.

https://github.com/fmontesi/cp2020/tree/master/exam

The webpage also contains more rules about the exam.

Note for people taking the re-exam of the 5 ECTS version of this course: you do not have to implement methods "longestWord" and "findWord".
*/
public class Exam {
	// Do not change this method
	public static void main(String[] args) {
		checkArguments(args.length > 0, "You must choose a command: help, shortestWord, longestWord, wordStartingWith, or findWord.");
		switch (args[0]) {
			case "help":
				System.out.println("Available commands: help, shortestWord, longestWord, wordStartingWith, or findWord.\nFor example, try:\n\tjava Exam shortestWord data");
				break;
			case "shortestWord":
				checkArguments(args.length == 2, "Usage: java Exam.java shortestWord <directory>");
				String shortestWord = shortestWord(Paths.get(args[1]));
				System.out.println("The shortest word found is " + shortestWord);
				break;
			case "longestWord":
				checkArguments(args.length == 2, "Usage: java Exam.java longestWord <directory>");
				String longestWord = longestWord(Paths.get(args[1]));
				System.out.println("The longest word found is " + longestWord);
				break;
			case "wordStartingWith":
				checkArguments(args.length == 4, "Usage: java Exam.java wordStartingWith <directory> <prefix> <true|false>");
				Optional<LocatedWord> locatedWordOptional = wordStartingWith(Paths.get(args[1]), args[2], Boolean.parseBoolean(args[3]));
				locatedWordOptional.ifPresentOrElse(
					locatedWord -> System.out.println("Found " + locatedWord.word + " in " + locatedWord.filepath ),
					() -> System.out.println("No match found")
				);
				break;
			case "findWord":
				checkArguments(args.length == 4, "Usage: java Exam.java findWord <directory> <word> <limit>");
				int limit = Integer.parseInt(args[3]);
				List<WordLocation> locations = findWord(Paths.get(args[1]), args[2], limit);
				if (locations.size() > limit) {
					throw new InternalException("Returned list size exceeds limit");
				}
				System.out.println("Found " + locations.size() + " matches");
				locations.forEach(location -> System.out.println(location.filepath + ":" + location.line));
				break;
			default:
				System.out.println("Unrecognised command: " + args[0] + ". Try java Exam.java help.");
				break;
		}
	}
	
	// Do not change this method
	private static void checkArguments(Boolean check, String message)
	{
		if (!check) {
			throw new IllegalArgumentException(message);
		}
	}

	/** Returns the shortest word present in the text files contained in a directory.
	 * 
	 * This method recursively visits a directory to find all the text files
	 * contained in it and its subdirectories (and the subdirectories of these
	 * subdirectories, etc.).
	 *
	 * You must consider only files ending with a ".txt" suffix. You are
	 * guaranteed that they will be text files.
	 * 
	 * The method should return the shortest word found among all text files.
	 * If multiple words are identified as shortest, the method should return
	 * the one that precedes the other shortest words lexicographically.
	 * To compare strings lexicographically, you can use String::compareTo.
	 * See also https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/lang/String.html#compareTo(java.lang.String)
	 * 
	 * @param dir the directory to search
	 * @return the shortest word found among all text files inside of dir
	 */
	private static String shortestWord(Path dir) {
		// Contains the shortest word in all the files.
		String shortestWord = null;
        
        ExecutorService executor = ForkJoinPool.commonPool();
        ExecutorCompletionService<String> completionService = new ExecutorCompletionService<>(executor);

        try {
            long pendingTasks = 
                Files.walk(dir)
                    .filter(line -> isTxt(line))
                    .map(file ->
                        completionService.submit(() -> findShortestWordInFile(file))
                    )
                    .count();

			// Loops until all pending tasks have been finished.
            while (pendingTasks > 0) {
				Future<String> future = completionService.take();
				// Gets the shortest word in file.
				String shortestWordInFile = future.get();

				// Compares if the shortestWordInFile is shorter than shortestWord.
                if (compareWordsForShortest(shortestWordInFile, shortestWord) == shortestWordInFile) {
					// Replaces shortestWord
					shortestWord = shortestWordInFile;
                }
                pendingTasks--;
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

        return shortestWord;
    }
    
    /**
	 * Checks if the path goes to a '.txt' file.
	 * 
	 * @param path Path.
	 * @return true if the path goes to a '.txt' file.
	 */
	private static boolean isTxt(Path path) {
		return (path.toString().endsWith(".txt"));
	}

	/**
     * Goes through the file and finds the word with the shortest length.
     * 
     * @param file Path for the file.
     * @return String of the word with shortest length.
     */
    private static String findShortestWordInFile(Path file) {
		// Contains the shortest word in the file.
		String[] shortestWordInFile = new String[1];

        try {
            Files.lines(file)
                .flatMap(line -> extractWords(line))
                .forEach(word -> {
					// If word is shorter than shortestWordInFile, replace shortestWordInFile with word
                    if (compareWordsForShortest(word, shortestWordInFile[0]) == word) {
                        shortestWordInFile[0] = word;
                    }
                });
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
        return shortestWordInFile[0];
	}

	/**
     * Compares the two words in length and lexicographically and returns the shortest.
	 * If one of the words is null, the other one is returned.
	 * If both words are null, null is returned.
     * 
     * @param word1 String
     * @param word2 String
     * @return The word which has the shortest length or if they are of equal length, the one who comes lexicographically first.
     */
    private static String compareWordsForShortest(String word1, String word2) {
        if (word1 == null && word2 == null) {
            return null;
        } else if (word1 == null) {
            return word2;            
        } else if (word2 == null) {
            return word1;
        } else {
            return (word1.length() < word2.length() || word1.length() == word2.length() && word1.compareTo(word2) < 0 ? word1 : word2);
        }
    }

	/** Returns the longest word present in the text files contained in a directory.
	 * 
	 * This method recursively visits a directory to find all the text files
	 * contained in it and its subdirectories (and the subdirectories of these
	 * subdirectories, etc.).
	 *
	 * You must consider only files ending with a ".txt" suffix. You are
	 * guaranteed that they will be text files.
	 * 
	 * The method should return the longest word found among all text files.
	 * If multiple words are identified as longest, the method should return
	 * the one that precedes the other longest words lexicographically.
	 * To compare strings lexicographically, you can use String::compareTo.
	 * See also https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/lang/String.html#compareTo(java.lang.String)
	 * 
	 * @param dir the directory to search
	 * @return the longest word found among all text files inside of dir
	 */
	private static String longestWord(Path dir) {
		String longestWord = null;
        
        ExecutorService executor = ForkJoinPool.commonPool();
        ExecutorCompletionService<String> completionService = new ExecutorCompletionService<>(executor);

        try {
            long pendingTasks = 
                Files.walk(dir)
                    .filter(path -> isTxt(path))
                    .map(file ->
                        completionService.submit(() -> findLongestWordInFile(file))
                    )
                    .count();

            while (pendingTasks > 0) {
                Future<String> future = completionService.take();
                String longestWordInFile = future.get();
                if (compareWordsForLongest(longestWordInFile, longestWord) == longestWordInFile) {
                    longestWord = longestWordInFile;
                }
                pendingTasks--;
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

        return longestWord;
    }
    
    /**
     * Goes through the file and finds the word with the greatest length.
     * 
     * @param file Path for the file.
     * @return String of the word with greates length.
     */
    private static String findLongestWordInFile(Path file) {
        String[] longestWordInFile = new String[1];

        try {
            Files.lines(file)
                .flatMap(path -> extractWords(path))
                .forEach(word -> {
                    if (compareWordsForLongest(word, longestWordInFile[0]) == word) {
                        longestWordInFile[0] = word;
                    }
                });
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
        return longestWordInFile[0];
	}

    /**
     * Extracts all the words from the line as a Stream.
     * 
     * @param line String which may contain multiple words.
     * @return Stream of Strings of all the words in the line
     */
    public static Stream<String> extractWords(String line) {
        List<String> words = new ArrayList<>();
		
		BreakIterator it = BreakIterator.getWordInstance();
		it.setText(line);
		
		int start = it.first(); // Starting boundary
		int end = it.next(); // Ending boundary
		while(end != BreakIterator.DONE) { // Loops until the BreakIterator has reached the end of the line.
			String word = line.substring(start, end);
			// If the String is a word or digit add it to the list.
			if (Character.isLetterOrDigit(word.charAt(0))) {
				words.add(word); 
			}
			// Goes to the next word.
			start = end;
			end = it.next();
		}
		
		return words.stream();
	}

    /**
     * Compares the two words in length and lexicographically and returns the longest.
	 * If one of the words is null, the other one is returned.
	 * If both words are null, null is returned.
     * 
     * @param word1 String
     * @param word2 String
     * @return The word which is the longest or if they are of equal length, the one who comes lexicographically first.
     */
    private static String compareWordsForLongest(String word1, String word2) {
        if (word1 == null && word2 == null) {
            return null;
        } else if (word1 == null) {
            return word2;            
        } else if (word2 == null) {
            return word1;
        } else {
            return (word1.length() > word2.length() || word1.length() == word2.length() && word1.compareTo(word2) < 0 ? word1 : word2);
        }
    }

	/** Returns an Optional<LocatedWord> (see below) about a word starting with the given prefix found in the files of the given directory.
	 * 
	 * This method recursively visits a directory to find text files
	 * contained in it and its subdirectories (and the subdirectories of these
	 * subdirectories, etc.).
	 *
	 * You must consider only files ending with a ".txt" suffix. You are
	 * guaranteed that they will be text files.
	 * 
	 * The method should return an (optional) LocatedWord object (defined by
	 * the class at the end of this file) containing:
	 *   - the word found that starts with the given prefix;
	 *   - the path to the file containing the word.
	 * 
	 * If the caseSensitive parameter is true, then the found word must start
	 * exactly with the given prefix. Otherwise, if caseSensitive is false,
	 * then the found word can start with the given prefix without considering
	 * differences between uppercase and lowercase letters.
	 * 
	 * If a word satisfying the description above can be found, then the method
	 * should return an Optional containing the desired LocatedWord.
	 * Otherwise, if such a word cannot be found, the method should
	 * return Optional.empty().
	 * 
	 * This method should return *as soon as possible*: as soon as a satisfactory
	 * word is found, the method should return a result without waiting for the 
	 * processing of remaining files and/or other data.
	 * 
	 * @param dir the directory to search
	 * @param prefix the prefix the word searched for should start with
	 * @param caseSensitive whether the search should be case sensitive
	 * @return an optional LocatedWord about a word starting with the given prefix
	 */
	private static Optional<LocatedWord> wordStartingWith(Path dir, String prefix, boolean caseSensitive) {
		AtomicReference<LocatedWord> locatedWord = new AtomicReference<>(null);
        AtomicBoolean wordFound = new AtomicBoolean(false);

		ExecutorService executor = ForkJoinPool.commonPool();
		
        try {
            Files.walk(dir)
                .filter(path -> isTxt(path))
                .map(file -> executor
                    .submit(() -> findWordStartingWithInFile(file, prefix, caseSensitive, wordFound, locatedWord))
				)
				.collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        try {
            executor.shutdown();
            executor.awaitTermination(1, TimeUnit.DAYS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
		
		return (wordFound.get() ? Optional.of(locatedWord.get()) : Optional.empty());
    }
    
    /**
	 * Goes though the file a sets the locatedWord to the word if there is a word that starts with the prefix.
	 * 
	 * @param file The path to the file.
	 * @param prefix The prefix that the word should start with.
	 * @param caseSensitive True if the prefix should be case sensitive to the word.
	 * @param wordFound Control variable to check whether a word has been found
	 * @param locatedWord Reference to the found word.
	 */
	private static void findWordStartingWithInFile(Path file, String prefix, boolean caseSensitive, AtomicBoolean wordFound, AtomicReference<LocatedWord> locatedWord) {
        try {
            Files.lines(file)
                .flatMap(line -> extractWords(line))
                .forEach(word -> {
					if (wordFound.get())
						throw new CancellationException();
			
                    if (startsWithCaseSensitive(word, prefix, caseSensitive) && wordFound.compareAndSet(false, true)) {
						locatedWord.set(new LocatedWord(word, file));
						throw new CancellationException();
					}
                });
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
	 * Returns true if the word starts with prefix.
	 * 
	 * @param word The word that is being compared to the prefix.
	 * @param prefix Prefix of word that is being searched for.
	 * @param caseSensitive True if the prefix should be case sensitive to the word.
	 * @return true if the word starts with the prefix
	 */
	private static boolean startsWithCaseSensitive(String word, String prefix, Boolean caseSensitive) {
		if (caseSensitive) {
			return word.startsWith(prefix);
		} else {
			return word.toLowerCase().startsWith(prefix.toLowerCase());
		}
	}

	/** Returns a list of locations at which the given word has been found.
	 * 
	 * This method recursively visits a directory to find text files
	 * contained in it and its subdirectories (and the subdirectories of these
	 * subdirectories, etc.).
	 *
	 * You must consider only files ending with a ".txt" suffix. You are
	 * guaranteed that they will be text files.
	 * 
	 * The method should return a list of WordLocation objects (defined by
	 * the class at the end of this file) containing:
	 *   - the line number at which the word has been found;
	 *   - the path to the file containing the word.
	 *
	 * The search is case sensitive, in the sense that the word found must match
	 * exactly the word given as parameter, also considering casing.
	 * For example, the word "Hello" would be judged as different from the word "hello",
	 * and the word "heY" would be different than the word "hEy".
	 * 
	 * The size of the returned list must not exceed the given limit.
	 * Therefore, this method should return *as soon as possible*: if the list
	 * reaches the given limit at any point during the computation, no more
	 * elements should be added to the list and remaining files and/or other lines
	 * should not be analysed.
	 * 
	 * @param dir the directory to search
	 * @param word the word to search for
	 * @param limit the size limit for the returned list
	 * @return a list of locations where the given word has been found
	 */
	private static List<WordLocation> findWord(Path dir, String word, int limit) {
		List<WordLocation> wordLocations = new ArrayList<>();

		AtomicInteger wordsAdded = new AtomicInteger(0);
		AtomicBoolean cancelled = new AtomicBoolean(false);

		ExecutorService executor = ForkJoinPool.commonPool();

		try {
			Files.walk(dir)
				.filter(path -> isTxt(path))
				.forEach(file -> {
					if (!cancelled.get()) {
						executor.submit(() -> checkFileForWord(file, word, wordLocations, wordsAdded, limit, cancelled));
					}
				});
		} catch (IOException e) {
			e.printStackTrace();
		}

		try {
			executor.shutdown();
			executor.awaitTermination(1, TimeUnit.DAYS);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		return wordLocations;
    }

	/**
	 * Creates a stream of Strings of the lines in the file and checks if the word is in that line.
	 * 
	 * @param file Path to the file.
	 * @param word The word that is being search for.
	 * @param wordLocations The list of all the already found WordLocations.
	 * @param wordsAdded The total amount of words that have been added to wordLocations.
	 * @param limit	The limit for how many words that can be added to wordLocations
	 * @param cancelled	Control variable to check if the limit has been reached.
	 */
	private static void checkFileForWord(Path file, String word, List<WordLocation> wordLocations, AtomicInteger wordsAdded, int limit, AtomicBoolean cancelled) {
		try {
			int[] lineCounter = {0};
			Files.lines(file)
				.map(line -> {
					return new LineInfo(++lineCounter[0], line, file); // Maps each line to LineInfo
				})
				.forEach(line -> {
					if (!cancelled.get()) {
						checkLineForWord(line, word, wordLocations, wordsAdded, limit, cancelled);
					}
				});
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

    /**
	 * Goes through the file and adds the WordLocation to wordLocations if the word is in the line. 
	 * If cancelled is true the limit has been reached and stops the search for more words.
	 * 
	 * @param lineInfo The information about the line.
	 * @param word The word that is being search for.
	 * @param wordLocations The list of all the already found WordLocations.
	 * @param wordsAdded The total amount of words that have been added to wordLocations.
	 * @param limit	The limit for how many words that can be added to wordLocations
	 * @param cancelled	Control variable to check if the limit has been reached.
	 */
	private static void checkLineForWord(LineInfo lineInfo, String word, List<WordLocation> wordLocations, AtomicInteger wordsAdded, int limit, AtomicBoolean cancelled) {
		String line = lineInfo.getLine();

		BreakIterator breakIterator = BreakIterator.getWordInstance(); // BreakIterator that breaks at every word.
		breakIterator.setText(line);

		int start = breakIterator.first(); // Starting boundary of word
		int end = breakIterator.next(); // Ending boundary of word

		while (end != BreakIterator.DONE) { // Iterates until the BreakIterator has reached the end of the line.
            

			// If cancelled stop the thread.
            if (cancelled.get()) 
                throw new CancellationException();

                
				
			
            String nextWord = line.substring(start, end);

			// Checks if the String isn't a seperator and it's equal to word.
			if (Character.isLetterOrDigit(nextWord.charAt(0)) && word.equals(nextWord)) {
                
                synchronized (wordLocations) {
                    // If the limit hasn't been reached and not cancelled hasn't been set to true.
					if (!cancelled.get() && limit != wordsAdded.get()) {

						// Adds nextWord to wordLocations
						wordLocations.add(new WordLocation(lineInfo.getPath(), lineInfo.getLineNumber()));

                        // If limit reached, set cancelled to true and throw CancellationException to stop thread.
						if (wordsAdded.incrementAndGet() == limit) {
							cancelled.set(true);
							throw new CancellationException();
						}
					}
				}
			}

			// Moves to the next word in the line.
			start = end;
			end = breakIterator.next();
		}
    }

    
    /**
	 * Class which contains which file it is from, it's line number and the line it slef
	 */
	private static class LineInfo {
		private int lineNumber;
		private String line;
		private Path path;

		/**
		 * 
		 * @param lineNumber The line number of the line in the file.
		 * @param line The line itself.
		 * @param path Path to the file which contains the line.
		 */
		private LineInfo(int lineNumber, String line, Path path) {
			this.lineNumber = lineNumber;
			this.line = line;
			this.path = path;
		}

		/**
		 * Getter for line.
		 * 
		 * @return Line.
		 */
		public String getLine() {
			return line;
		}

		/**
		 * Getter for lineNumber.
		 * 
		 * @return Line number.
		 */
		public int getLineNumber() {
			return lineNumber;
		}

		/**
		 * Getter for path
		 * 
		 * @return Path to the file.
		 */
		public Path getPath() {
			return path;
		}
	}

	// Do not change this class
	private static class LocatedWord {
		private final String word; // the word
		private final Path filepath; // the file where the word has been found

		private LocatedWord(String word, Path filepath) {
			this.word = word;
			this.filepath = filepath;
		}
	}

	// Do not change this class
	private static class WordLocation {
		private final Path filepath; // the file where the word has been found
		private final int line; // the line number at which the word has been found

		private WordLocation(Path filepath, int line) {
			this.filepath = filepath;
			this.line = line;
		}
	}
	
	// Do not change this class
	private static class InternalException extends RuntimeException {
		private InternalException(String message) {
			super(message);
		}
	}
}