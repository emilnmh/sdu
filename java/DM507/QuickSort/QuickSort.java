import java.util.Random;

public class QuickSort {
    public static void main(String[] arg) {
        int[] randomArray = generateRandomIntArray(10000);
        quickSort(randomArray);
        for (int i : randomArray) {
            System.out.println(i);
        }
    }

    public static void quickSort(int[] array) {
        quickSort(array, 0, array.length - 1);
    }

    public static void quickSort(int[] array, int p, int r) {
        if (p < r) {
            int q = partition(array, p, r);
            quickSort(array, p, q - 1);
            quickSort(array, q + 1, r);
        }
    }

    private static int partition(int[] array, int p, int r) {
        int x = array[r];
        int i = p - 1;
        for (int j = p; j < r; j++) {
            if (array[j] <= x) {
                i = i + 1;
                swap(array, i, j);
            }
        }
        swap(array, i + 1, r);
        return i + 1;
    }

    private static void swap(int[] array, int i, int j) {
        int e = array[i];
        array[i] = array[j];
        array[j] = e;
    }

    /**
     * Generates random int arrays with values from 0 to 1.000.000
     * @param n as the array size
     * @return Random int array.
     */
    public static int[] generateRandomIntArray(int n) {
        int[] array = new int[n];
        Random rand = new Random();

        for (int i = 0; i < n; i++) {
            array[i] = rand.nextInt(1000000);
        }

        return array;
    }
}