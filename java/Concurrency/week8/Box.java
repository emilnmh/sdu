public class Box<T> {
    private T content;

    /*
     * Constructor
     */
    public Box(T content) {
        this.content = content;      
    }

    /*
     * Returns content of Box.
     */
    public T content() {
        return content;
    }

    /*
     * Applies Boxfunction apply() to content of box and returns type of O.
     */
    public <O> O apply(BoxFunction<T, O> boxFunction) {
        return boxFunction.apply(content);
    }

}