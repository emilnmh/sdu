import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Collectors;

public class ThreadsExercise21 {
    /*
	Modify Threads/cp/WalkParallelStreamFindAny such that it returns a boolean
	telling whether there exists at least one file with more than 10 lines.
    */
    public static void main(String[] args) {
		try {
			boolean fileWithMoreThan10Lines =
				Files.walk(Paths.get("data"))
					.filter(Files::isRegularFile)
					.collect(Collectors.toList())
					.parallelStream()
					.anyMatch(file -> {
                        try {
                            return (Files.lines(file).count() > 10);
                        } catch (IOException e) {
                            e.printStackTrace();
                            return false;
                        }
                    });
			System.out.println(fileWithMoreThan10Lines);
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
}