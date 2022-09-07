import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class AdvancedBox<T> {
    private final T content;

    public AdvancedBox(T content) {
        if (content == null) {
            throw new IllegalArgumentException("Object is null");
        }
        this.content = content;
    }

    public T content() {
        return content;
    }

    public <O> O apply(BoxFunction<T, O> boxFunction) {
        return boxFunction.apply(content);
    }


    public static void main(String[] arg) {
        List<String> stringList = Arrays.asList(listElements());
        Box<List<String>> advancedBox = new Box<>(stringList);
        advancedBox.apply((l) -> {
            l.sort((s1, s2) -> s1.compareTo(s2));
            return l;
        });
        System.out.println(stringList);

        int sumOfLengths = advancedBox.apply((l) -> {
            int sum = 0;
            for (String s : l) {
                sum += s.length();
            }
            return sum;
        });

        System.out.println(sumOfLengths);
    }

    public static String[] listElements() {
        String[] list = {"Hi", "Hej", "Ciao", "Ola", "Hallo", "Hola"};
        return list;
    }
}