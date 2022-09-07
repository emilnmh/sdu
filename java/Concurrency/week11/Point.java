public class Point {

    public static void main() {
        Counter c = new Counter();
        Point p = new Point();

        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                p.c1.increment();
            }
        });

        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 100000; i++) {
                p.c2.increment();
            }
        });

        Thread t3 = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                if (p.areEqual()) {
                    c.increment();
                };
            }
        });

        t1.start();
        t2.start();
        t3.start();

        try {
            t1.join();
            t2.join();
            t3.join();
            System.out.println(c.count());
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public final Counter c1;
    public final Counter c2;

    public Point() {
        c1 = new Counter();
        c2 = new Counter();
    }

    public boolean areEqual() {
        System.out.println(c1.count() + " = " + c2.count() + " - " + (c1.count() == c2.count()));
        return (c1.count() == c2.count());
    }
}