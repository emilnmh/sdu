import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Stream;

public class SynchronizedMap2T
{
	public static void main()
	{
		Map<String, Integer> result1 = new HashMap<>();
		Map<String, Integer> result2 = new HashMap<>();
		
		Thread t1 = new Thread( () -> {
			try {
				Files.lines( Paths.get( "text1.txt" ) )
					.flatMap( s -> Stream.of( s.split( " " ) ) )
					.forEach( word -> {
							result1.merge( word, 1, Integer::sum );
						}
					);
			} catch( IOException e ) {
				e.printStackTrace();
			}
		});
		
		Thread t2 = new Thread( () -> {
			try {
				Files.lines( Paths.get( "text2.txt" ) )
					.flatMap( s -> Stream.of( s.split( " " ) ) )
					.forEach( word -> {
						result2.merge( word, 1, Integer::sum );
					} );
			} catch( IOException e ) {
				e.printStackTrace();
			}
		});
		
		t1.start();
		t2.start();
		try {
			t1.join();
			t2.join();
			
		} catch( InterruptedException e ) {
			e.printStackTrace();
		}

		Map<String, Integer> finalResult = new HashMap<>();
		finalResult.putAll(result1);

		result2.forEach((key, value) -> finalResult.merge(key, value, Integer::sum));
		System.out.println(finalResult);
	}
}
