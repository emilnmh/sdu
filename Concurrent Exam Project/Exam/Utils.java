public class Utils
{
	public static void doAndMeasure( Runnable runnable )
	{
		long t1 = System.currentTimeMillis();
		runnable.run();
		long t2 = System.currentTimeMillis();
		System.out.println( "Elapsed time: " + (t2-t1) + "ms" );	
	}
	
	public static void benchmark( Runnable runnable )
	{
		runnable.run();
		long t1 = System.currentTimeMillis();
		runnable.run();
		long t2 = System.currentTimeMillis();
		System.out.println( "Elapsed time: " + (t2-t1) + "ms" );	
	}
	
	public static void repeat( int times, Runnable runnable )
	{
        long totalTime = 0;
		for( int i = 0; i < times; i++ ) {
            long t1 = System.currentTimeMillis();
            runnable.run();
            long t2 = System.currentTimeMillis();
            totalTime += t2 - t1;
        }
        
        System.out.println("Average time: " + totalTime / times + "ms");
	}
}