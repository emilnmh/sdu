import java.util.Random;

public class InsertionSort {
    public static void main(String[] args) {
        int[] randomArray = randomArray(100000);
        long startTime = System.currentTimeMillis();
        sort(randomArray);
        System.out.println("Program time: " + (System.currentTimeMillis() - startTime) + "ms");
    }

    /*
     * Sorts the array with InsertionSort
     */
    public static void sort(int[] array) {
        int lengthPointer = 1; // controls how far the array has been sorted.
        while (lengthPointer < array.length) {
            int i = lengthPointer;
            while (i >= 1 && array[i] < array[i-1]) { 
                swap(array, i, i-1); // Swaps elements places
                i--;
            }
            lengthPointer++;
        }
    }

    /*
     * Swaps to elements in the array.
     */
    private static void swap(int[] a, int i, int j) {
        int k = a[i];
        a[i] = a[j];
        a[j] = k;
    }

    /*
     * Generates random array of ints
     */
     private static int[] randomArray(int n) {
         int[] a = new int[n];
         Random rand = new Random();
         for (int i = 0; i < n; i++) {
             a[i] = rand.nextInt();
         }
         return a;
     }
}