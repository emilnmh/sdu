import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;


public class ThreadsExercise15
{
	/*
	Adapt your program from ThreadsExercise14 to use an ExecutorCompletionService, as in Threads/cp/WalkCompletionService.
	*/
    
    public static void main(String[] args)
	{
		// word -> number of times it appears over all files
		Map< Path, FileInfo > infoForFile = new HashMap<>();
		ExecutorService executor = Executors.newWorkStealingPool();
		ExecutorCompletionService<FileInfoWithPath> completionService = new ExecutorCompletionService<>(executor);

		try {
			long pendingTasks = 
				Files.walk(Paths.get("data"))
					.filter(Files::isRegularFile)
					.map(filepath ->
						completionService.submit(() -> getFileInfo(filepath))
					)
                    .count();
			while (pendingTasks > 0) {
				FileInfoWithPath fileInformation = completionService.take().get();
				infoForFile.put(fileInformation.path, fileInformation.fileInfo);
				pendingTasks--;
			}
		} catch( InterruptedException | ExecutionException | IOException e ) {
			e.printStackTrace();
		}
		
		try {
			executor.shutdown();
			executor.awaitTermination( 1, TimeUnit.DAYS );
		} catch( InterruptedException e ) {
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