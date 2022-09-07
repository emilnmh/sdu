public class Main {
    public static void main(String[] arg) {

        /*
        Counter counter = new Counter();
        
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                counter.increment();
            }
        });
        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                counter.decrement();
            }
        });
        Thread t3 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                counter.increment();
            }
        });

        t1.start();
        t2.start();
        t3.start();

        try {
            t1.join();
            t2.join();
            t3.join();    
            System.out.println(counter.count());
        } catch (InterruptedException e) {
            e.printStackTrace();
        }*/
        SynchronizedMap2T.main();
    }
}