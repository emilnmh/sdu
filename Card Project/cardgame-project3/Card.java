public enum Card {

    ACE_HEARTS(Suit.HEARTS, "Ace", 11),
    SEVEN_HEARTS(Suit.HEARTS, "Seven", 10),
    KING_HEARTS(Suit.HEARTS, "King", 4), // Continued. 40 rows in total.
    JACK_HEARTS(Suit.HEARTS, "Jack", 3),
    QUEEN_HEARTS(Suit.HEARTS, "Queen", 2),
    SIX_HEARTS(Suit.HEARTS, "Six", -1),
    FIVE_HEARTS(Suit.HEARTS, "Five", -2),
    FOUR_HEARTS(Suit.HEARTS, "Four", -3),
    THREE_HEARTS(Suit.HEARTS, "Three", -4),
    TWO_HEARTS(Suit.HEARTS, "Two", -5),
    ACE_CLUBS(Suit.CLUBS, "Ace", 11),
    SEVEN_CLUBS(Suit.CLUBS, "Seven", 10),
    KING_CLUBS(Suit.CLUBS, "King", 4), // Continued. 40 rows in total.
    JACK_CLUBS(Suit.CLUBS, "Jack", 3),
    QUEEN_CLUBS(Suit.CLUBS, "Queen", 2),
    SIX_CLUBS(Suit.CLUBS, "Six", -1),
    FIVE_CLUBS(Suit.CLUBS, "Five", -2),
    FOUR_CLUBS(Suit.CLUBS, "Four", -3),
    THREE_CLUBS(Suit.CLUBS, "Three", -4),
    TWO_CLUBS(Suit.CLUBS, "Two", -5),
    ACE_SPADES(Suit.SPADES, "Ace", 11),
    SEVEN_SPADES(Suit.SPADES, "Seven", 10),
    KING_SPADES(Suit.SPADES, "King", 4), // Continued. 40 rows in total.
    JACK_SPADES(Suit.SPADES, "Jack", 3),
    QUEEN_SPADES(Suit.SPADES, "Queen", 2),
    SIX_SPADES(Suit.SPADES, "Six", -1),
    FIVE_SPADES(Suit.SPADES, "Five", -2),
    FOUR_SPADES(Suit.SPADES, "Four", -3),
    THREE_SPADES(Suit.SPADES, "Three", -4),
    TWO_SPADES(Suit.SPADES, "Two", -5),
    ACE_DIAMONDS(Suit.DIAMONDS, "Ace", 11),
    SEVEN_DIAMONDS(Suit.DIAMONDS, "Seven", 10),
    KING_DIAMONDS(Suit.DIAMONDS, "King", 4), // Continued. 40 rows in total.
    JACK_DIAMONDS(Suit.DIAMONDS, "Jack", 3),
    QUEEN_DIAMONDS(Suit.DIAMONDS, "Queen", 2),
    SIX_DIAMONDS(Suit.DIAMONDS, "Six", -1),
    FIVE_DIAMONDS(Suit.DIAMONDS, "Five", -2),
    FOUR_DIAMONDS(Suit.DIAMONDS, "Four", -3),
    THREE_DIAMONDS(Suit.DIAMONDS, "Three", -4),
    TWO_DIAMONDS(Suit.DIAMONDS, "Two", -5);

    public final Suit suit;
    private final String name;
    private final int value;

    /*
     * Constructor
     */
    private Card(Suit suit, String name, int value) {
        this.suit = suit;
        this.name = name;
        this.value = value;
    }

    /* 
     * Returns the number of points this card is worth. 
     */
    public int points() {
        return ((this.value > 0) ? value : 0);
    }

    /*
     * Returns true if this card is higher than other card.
     */
    public boolean isHigherThan(Card other) {
        return (this.value > other.value);
    }

    /*
     * Returns a textural representation
     */
    public String toString() {
        return this.name + " of " + this.suit.toString();
    }

    /*
     * Getter for name.
     */
    public String getName() {
        return name;
    }
}