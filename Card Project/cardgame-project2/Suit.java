
public enum Suit {
    HEARTS("Hearts"),
    CLUBS("Clubs"),
    SPADES("Spades"),
    DIAMONDS("Diamonds");

    private final String suit;

    /*
     * Constructor 
     */
    private Suit(String suit) {
        this.suit = suit;
    }

    /*
     * Returns a textural representation
     */
    public String toString() {
        return suit;
    }
}