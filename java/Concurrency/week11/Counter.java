public class Counter {
    public static void main() {
        Counter counter = new Counter();


        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                counter.increment();
            }
        });

        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                counter.increment();
            }
        });

        Thread t3 = new Thread(() -> {
            
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
        }
    }

    private int i;

    public Counter() {
        i = 0;
    }

    public synchronized void increment() {
        i++;
    }

    public synchronized void decrement() {
        i--;
    }

    public synchronized int count(){
        return i;
    }
}