import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;
import java.util.stream.Collectors;

public class ThreadsExercise20 {
    /*
	Modify ThreadsExercise19 such that the computed map contains only
	entries for files that have at least 10 lines.
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
                                    if (line.startsWith("L")) {
                                        fileInfo.incrementLLines();
                                    }
                                });
                            return fileInfo;
                        } catch (IOException e) {
                            e.printStackTrace();
                            return new FileInfo(Paths.get(""), 0);
                        }
                    })
                    .filter(fileInfo -> fileInfo.getLines() >= 10)
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