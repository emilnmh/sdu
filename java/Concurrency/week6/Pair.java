import java.util.ArrayList;

public class Pair<K, V> {
    public static void main(String[] args) {
        String string = "Test String";
        int number = 42;
        Pair<String, Integer> pair = new Pair<>(string, number);
        System.out.println(pair);

        ArrayList<Pair<String, Integer>> pairs = new ArrayList<Pair<String, Integer>>();
        pairs.add(new Pair<String, Integer>("test", 110));
        pairs.add(new Pair<String, Integer>("test", 11));
        pairs.add(new Pair<String, Integer>("tester", 1));
        pairs.add(new Pair<String, Integer>("testere", 110));
        pairs.add(new Pair<String, Integer>("tester", 1));

        ArrayList<String> usedKeys = new ArrayList<String>();
        for (Pair<?, ?> p1 : pairs) {
            if (!usedKeys.contains(p1.key())) {
                usedKeys.add((String)p1.key());
                int sum = 0;
                for (Pair<?, ?> p2 : pairs) {
                    if (p1.key().equals(p2.key())) {
                        sum = sum + (Integer)p2.value();
                    }
                }
                System.out.println(p1.key() + ": " + sum);
            }
        }
    }

    private K key;
    private V value;

    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    public K key() {
        return key;
    }

    public V value() {
        return value;
    }

    public String toString() {
        return key.toString() + " is associated to: " + value.toString();
    }
}