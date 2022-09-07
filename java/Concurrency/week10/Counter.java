public final class Counter {
    private int i;
    private final Object lock = new Object();

    public Counter() {
        i = 0;
    }

    public void increment() {
        synchronized (lock) {
            i++;
        }
    }

    public void decrement() {
        synchronized (lock) {
            i--;
        }
    }

    public int count() {
        return i;
    }
}