import java.nio.file.Path;

public class FileInfo {
    private Path filePath;
    private long fileSize;
    private long numberOfLines;
    private long numberOfLinesStartingWithL;

    public FileInfo(Path path, long fileSize) {
        this.filePath = path;
        this.fileSize = fileSize;
        numberOfLines = 0;
        numberOfLinesStartingWithL = 0;
    }

    public void incrementLines() {
        numberOfLines++;
    }

    public void incrementLLines() {
        numberOfLinesStartingWithL++;
    }

    public Path getPath() {
        return filePath;
    }

    public long getFileSize() {
        return fileSize;
    }

    public long getLines() {
        return numberOfLines;
    }

    public long getLLines() {
        return numberOfLinesStartingWithL;
    }

    public String toString() {
        return filePath.toString() + "\nFile Size: " + fileSize + "\nNumber of Lines: " + numberOfLines + "\nNumber of Lines Starting with L: " + numberOfLinesStartingWithL;
    }
}