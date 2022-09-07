
import java.util.Scanner;

public class PQSort { 
    public static void main(String[] args) {
		PQ pq = new PQHeap();
		int n = 0;
		Scanner sc = new Scanner(System.in);
		while (sc.hasNextInt()) {
			pq.insert(new Element(sc.nextInt(),null));
			n++;
		}
		while (n > 0){
			System.out.println(pq.extractMin().getKey());
			n--;
		}








		
		/*
		Scanner sc = new Scanner(System.in);
		int[] array = new int[63];
		int n = 0;
		while (sc.hasNextInt()) {
			array[n] = sc.nextInt();
			n++;
		}
		for (int i = 0; i < array.length / 2; i++) {
			System.out.println(array[i] + " is parent to: " + array[i * 2 + 1] + " & " + array[i * 2 + 2]);
		}
		*/
   }
}