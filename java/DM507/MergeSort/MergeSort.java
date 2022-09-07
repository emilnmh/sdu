import java.util.Random;;

public class MergeSort {
    public static void main(String[] args) {
        long averageTime = 0;
        int iterations = 10;
        int arraySize = 200000;

        for (int i = 0; i < iterations; i++) {
            int[] array = generateRandomIntArray(arraySize);
            long time = System.currentTimeMillis();
            mergeSort(array);
            long endTime = System.currentTimeMillis();
            System.out.println("Array Size: " + arraySize + " - Time spent sorting " + (endTime - time) + "ms");
            averageTime += (endTime - time);
        }
        System.out.println("Average time for mergesort of " + arraySize + " elements: " + averageTime / iterations + " ms");
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

    public static void mergeSort(int[] array) {
        mergeSortAlgorithm(array, 0, array.length - 1);
    }

    private static void mergeSortAlgorithm(int[] array, int l, int r) {
        if (l < r) {
            int m = (l+r) / 2; // middle
            mergeSortAlgorithm(array, l, m);
            mergeSortAlgorithm(array, m+1, r);
            merge(array, l, m, r);
        }
    }

    private static void merge(int[] array, int l, int m, int r) {
        int n1 = m - l + 1; 
        int n2 = r - m; 

        int L[] = new int [n1]; 
        int R[] = new int [n2];

        for (int i=0; i < n1; ++i) {
            L[i] = array[l + i]; 
        }
            
        for (int j=0; j < n2; ++j) {
            R[j] = array[m + 1+ j];
        }
        
        int i = 0, j = 0; 
  
        // Initial index of merged subarry array 
        int k = l; 
        while (i < n1 && j < n2) 
        { 
            if (L[i] <= R[j]) { 
                array[k] = L[i]; 
                i++; 
            } else { 
                array[k] = R[j]; 
                j++; 
            } 
            k++; 
        } 
  
        /* Copy remaining elements of L[] if any */
        while (i < n1) { 
            array[k] = L[i]; 
            i++; 
            k++; 
        } 
  
        /* Copy remaining elements of R[] if any */
        while (j < n2) { 
            array[k] = R[j]; 
            j++; 
            k++; 
        } 
    }
}