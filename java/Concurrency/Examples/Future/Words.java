import java.text.BreakIterator;
import java.util.stream.*;
import java.util.List;
import java.util.ArrayList;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;

public class Words {
    public static void main() {
        try {
            Files.lines( Paths.get( "text1.txt" ) )
                .flatMap( Words::extractWords ) // Every line to a stream of words.
                .forEach( System.out::println ); // Prints for every element of the stream.
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static Stream< String > extractWords(String s) {
        List< String > words = new ArrayList<>(); // The list of all the words.

        /* BreakIterator:
         * getCharacterInstance(); Breaks at every character.
         * getWordInstance(); Breaks at every word.
         * getLineInstance(); Breaks at every line.
         * getSentenceInstance(); Breaks at every sentence.
         */
        BreakIterator it = BreakIterator.getWordInstance(); 
        it.setText(s); // Sets the text.

        int start = it.first(); // Gets first boundary.
        int end = it.next(); // Gets next boundary.
        while( end != BreakIterator.DONE ) { // Iterate over the whole boundary.
            String word = s.substring( start, end );
            if ( Character.isLetterOrDigit( word.charAt( 0 ) ) ) { // Checks if it is a seperater (Like space).
                words.add( word );
            }
            start = end;
            end = it.next();
        }

        return words.stream(); // Returns the list as a stream.
    }
}