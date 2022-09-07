import java.util.concurrent.atomic.*;

public class CounterAtomic {
    private AtomicInteger i;

    public CounterAtomic() {
        i = new AtomicInteger();
    }

    public int increment() {
        return i.incrementAndGet();
    }

    public int decrement() {
        return i.decrementAndGet();
    }
}