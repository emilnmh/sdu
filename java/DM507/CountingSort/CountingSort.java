import java.util.Random;

public class CountingSort {
    public static void main(String[] arg) {
        int n = 500000;
        
        for (int i = 0; i < 20; i++) {
            int k = n * 50;
            int[] randomArray = generateRandomArray(n, k);
            countingSort(randomArray, k);
        }
        for (int i = 0; i < 10; i++) {
            int k = (i < 5 ? n / 50 : n * 50);
            int[] randomArray = generateRandomArray(n, k);

            long startTime = System.nanoTime();
            countingSort(randomArray, k);
            System.out.println((System.nanoTime() - startTime) / (n+k));
        }
        
    }

    public static int[] countingSort(int[] a, int maxValueInA) {
        int[] b = new int[a.length]; // Resulting array
        int[] c = new int[maxValueInA + 1]; // Temperary array

        for (int e : a) {
            c[e] = c[e] + 1;
        }
        
        for (int i = 1; i < c.length; i++) {
            c[i] = c[i] + c[i-1];
        }

        for (int i = a.length-1; i >= 0; i--) {
            b[c[a[i]] - 1] = a[i];
            c[a[i]] = c[a[i]] - 1;
        }

        return b;
    }

    public static int[] generateRandomArray(int n, int max) {
        int[] array = new int[n];
        Random rand = new Random();
        for (int i = 0; i < array.length; i++) {
            array[i] = rand.nextInt(max + 1);
        }
        return array;
    }
}