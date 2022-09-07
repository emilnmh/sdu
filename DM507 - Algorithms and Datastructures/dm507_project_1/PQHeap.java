/*
 * Students:
 *      Emiln19 - Emil Nygaard Mathisen Hansen
 *      Jole19 - Johannes Liem Quang Le
 */

import java.util.ArrayList;

public class PQHeap implements PQ {
    /*
     * Attributes
     */
    private ArrayList<Element> heap; // Min-heap structure.

    /*
     * Constructor
     */
    public PQHeap() {
        heap = new ArrayList<>();
    }

    /*
     * Removes the element with least priority and returns it.
     * Precondition: heap size > 0
     */
    public Element extractMin() {
        Element min = heap.get(0); 
        int i = heap.size() - 1; // index of last element
        swap(0, i); 
        heap.remove(i); 
        minHeapify(0);

        return min;
    }

    /*
     * Inserts element into the heap. 
     */
    public void insert(Element e) {
        heap.add(heap.size(), e);        
        int i = heap.size() - 1;

        // Swaps the new element with its parent, until the parent has less priority.
        while (i > 0 && key(parent(i))> key(i)) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    /*
     * Returns left child index of parent.
     */
    public int leftChild(int parentIndex) {
        return 2 * parentIndex + 1;
    }

    /*
     * Returns right child index of parent.
     */
    public int rightChild(int parentIndex) {
        return 2 * parentIndex + 2;
    }

    /*
     * Returns parent of child.
     */
    public int parent(int childIndex) {
        return (childIndex - 1) / 2;
    }

    /*
     * Heapifies heap to a min-heap.
     */
    private void minHeapify(int i) {
        int l = leftChild(i);
        int r = rightChild(i);
        
        // Finds the index of the element with least priority of i and l.
        int smallest = (l < heap.size() && key(l) < key(i) ? l : i);
        // Finds the index of the element with least priority of smallest and r.
        if (r < heap.size() && key(r) < key(smallest))
            smallest = r;

        // If the smallest isn't the i, swap i and smallest and recursively call heapify with smallest.
        if (smallest != i) {
            swap(i, smallest);
            minHeapify(smallest);
        }
    }

    /*
     * Swaps to elements.
     */
    private void swap(int i, int j) {
        Element e = heap.get(i);
        heap.set(i, heap.get(j));
        heap.set(j, e);
    }

    // Returns the key of the element.
    private int key(int i) {
        return heap.get(i).getKey();
    }
}