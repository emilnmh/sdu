import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;


public class ThreadsExercise16
{
	/*
	Adapt your program from ThreadsExercise15 to use CompletableFuture, as in Threads/cp/WalkCompletableFuture.
	*/
    
    public static void main(String[] args)
	{
		// word -> number of times it appears over all files
		Map< Path, FileInfo > infoForFile = new HashMap<>();

		try {
            CompletableFuture<Void>[] futures = 
				Files.walk(Paths.get("data"))
					.filter(Files::isRegularFile)
                    .map(filepath ->
                        CompletableFuture.supplyAsync(() -> getFileInfo(filepath))
                            .thenAccept(fileInformation -> 
                                infoForFile.put(fileInformation.path, fileInformation.fileInfo)
                            )
					)
                    .collect(Collectors.toList()).toArray(new CompletableFuture[0]);

            CompletableFuture.allOf(futures).join();
		} catch(IOException e) {
			e.printStackTrace();
		}
	
		
        infoForFile.forEach((k, v) -> System.out.println(k.toString() + " " + v.toString()));
	}
    


    private static FileInfoWithPath getFileInfo(Path textFile) {
        long fileSize = -1;
        long lines = 0;
        long linesWithL = 0;
        try {
            fileSize = Files.size(textFile);
    
			for (String line : Files.readAllLines(textFile)) {
                lines++;
                if (line.startsWith("L")) {
                    linesWithL++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
		}
        return new FileInfoWithPath(new FileInfo(fileSize, lines, linesWithL), textFile);
    }
    
    
    private static class FileInfo {
        final long fileSize;
        final long numberOfLines;
        final long numberOfLinesStartingWithL;

        public FileInfo(long fileSize, long numberOfLines, long numberOfLinesStartingWithL) {
            this.fileSize = fileSize;
            this.numberOfLines = numberOfLines;
            this.numberOfLinesStartingWithL = numberOfLinesStartingWithL;
        }

        public String toString() {
            return String.format("(filesize:%d, numlines:%d, num_l_lines:%d)", fileSize, numberOfLines, numberOfLinesStartingWithL);
        }
    }

    private static class FileInfoWithPath {
        final FileInfo fileInfo;
        final Path path;

        public FileInfoWithPath(FileInfo fileInfo, Path path) {
            this.fileInfo = fileInfo;
            this.path = path;
        }
    }
}