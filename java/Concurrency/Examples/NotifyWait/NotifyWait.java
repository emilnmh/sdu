public class NotifyWait {
    
    private static Object monitor = new Object(); 
    private static boolean t2Done = false; // Used so t1 doesn't wait forever.

    /*
     * Make one thread print before the other.
     */
    public static void main() {
        Thread t1 = new Thread(() -> {
            System.out.println("Hello from t1");
            synchronized (monitor) {
                monitor.notify(); // Wakes up on thread who is waiting on the same object. (unpredictedly)
                t2Done = true;
            }
        });
        Thread t2 = new Thread(() -> {
            // T2 should wait util t1 has printed.
            synchronized (monitor) {
                if (!t2Done) {
                    try {
                        monitor.wait(); // Can wait forever if the other thread notified before wait was called. t2Done prevents this.
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
            System.out.println("Hello from t2");        
        });

        t1.start();
        t2.start();
        
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}