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


public class ThreadsExercise14
{
	/*
	Modify Threads/cp/WalkExecutorFuture such that, instead of word occurrences,
	it computes a map of type Map< Path, FileInfo >, which maps the Path of each file found in "data"
	to an object of type FileInfo that contains:
		- the size of the file;
		- the number of lines contained in the file;
		- the number of lines starting with the uppercase letter "L".
    */
    
    public static void main(String[] args)
	{
		// word -> number of times it appears over all files
		Map< Path, FileInfo > infoForFile = new HashMap<>();
		ExecutorService executor = Executors.newWorkStealingPool();

		try {
			List<Future<FileInfoWithPath>> futures =
				Files.walk( Paths.get( "data" ) )
					.filter( Files::isRegularFile )
					.map( filepath ->
						executor.submit( () -> getFileInfo( filepath ) )
					)
                    .collect( Collectors.toList());
                    
			for( Future<FileInfoWithPath> future : futures ) {
                FileInfoWithPath fileInformation = future.get();
                infoForFile.put(fileInformation.path, fileInformation.fileInfo);
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