/*
 * Class to test the different Java classes.
 */
public class Test {

    private static final Suit suitSpades = Suit.SPADES;
    private static final Suit suitHearts = Suit.HEARTS;
    private static final Suit suitClubs = Suit.CLUBS;
    private static final Suit suitDiamonds = Suit.DIAMONDS;
    private static final Suit anotherSuit = Suit.HEARTS;

    private static final Card card1 = Card.ACE_HEARTS;
    private static final Card card2 = Card.SIX_CLUBS;
    private static final Card card3 = Card.ACE_SPADES;
    private static final Card card4 = Card.KING_DIAMONDS;
    private static final Card card5 = Card.SEVEN_SPADES;

    private static Player player1 = new Player("Player1");

    public static void main(String[] args) {
        //Test the different methods here.

        // SUIT Methods
        System.out.println("--- SUITS ---");

        System.out.println(suitSpades.toString());
        System.out.println(suitHearts.toString());
        System.out.println(suitClubs.toString());
        System.out.println(suitDiamonds.toString());

        System.out.println(suitSpades.equals(anotherSuit));
        System.out.println(suitHearts.equals(anotherSuit));

        
        // CARD Methods
        System.out.println("--- CARDS ---");

        System.out.println(card1.toString() + ": " + card1.points());
        System.out.println(card2.toString() + ": " + card2.points());
        System.out.println(card3.toString() + ": " + card3.points());
        System.out.println(card4.toString() + ": " + card4.points());
        System.out.println(card5.toString() + ": " + card5.points());

        System.out.println(card1.toString() + " is higher than " +  card2.toString() + ": " + card1.isHigherThan(card2));
        System.out.println(card2.toString() + " is higher than " +  card3.toString() + ": " + card2.isHigherThan(card3));
        System.out.println(card1.toString() + " is higher than " +  card3.toString() + ": " + card1.isHigherThan(card3));

        System.out.println(card4.toString() + " suit is: " + card4.suit.toString());
        System.out.println(card4.toString() + " suit is: " + card4.suit.toString());

        // PLAYER Methods
        System.out.println("--- PLAYER ---");

        System.out.println("Player1 cards in hand: " + player1.cardsInHand());
        System.out.println("Player1 name: " + player1.name());
        player1.addToHand(card4);
        player1.addToHand(card2);
        player1.addToHand(card1);
        System.out.println("Player1 cards in hand: " + player1.cardsInHand());
    }
}