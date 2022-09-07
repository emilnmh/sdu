import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;
import java.util.stream.Collectors;

public class ThreadsExercise19 {
    /*
	Modify Threads/cp/WalkParallelStream5 to compute a Map< Path, FileInfo >
	that maps each file (the Path) to a FileInfo object that contains
		- the Path of the file;
		- the size of the file;
		- the number of lines contained in the file;
		- the number of lines starting with the uppercase letter "L".
    */
    public static void main(String[] args) {
		try {
            Map<Path, FileInfo> paths =
                Files.walk(Paths.get("data"))
                    .filter(Files::isRegularFile)
                    .collect(Collectors.toList())
                    .parallelStream()
                    .map(file -> {
                        try {
                            FileInfo fileInfo = new FileInfo(file, Files.size(file));
                            Files.lines(file)
                                .forEach(line -> {
                                    fileInfo.incrementLines();
                                    if (line.startsWith("L"))  {
                                        fileInfo.incrementLLines();
                                    }
                                });
                            return fileInfo;
                        } catch (IOException e) {
                            e.printStackTrace();
                            return new FileInfo(Paths.get(""), 0);
                        }
                    })
                    .collect(Collectors.toMap(
                        fileInfo -> fileInfo.getPath(),
                        fileInfo -> fileInfo
                    ));

        paths.forEach((path, info) -> System.out.println(info));
		} catch( IOException e ) {
			e.printStackTrace();
		}
	}
}