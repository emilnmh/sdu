import java.util.Arrays;
import java.util.Iterator;
import java.util.Random;
import java.util.NoSuchElementException;
import java.util.ArrayList;

public class Deck implements Iterable<Card> {

    private ArrayList<Card> cardsInDeck;
    private Random random;

    /*
     * Constructor
     */
    public Deck() {
        cardsInDeck = new ArrayList<Card>();
        random = new Random();

        ArrayList<Card> allExistingCards = new ArrayList<Card>();
        allExistingCards.addAll(Arrays.asList(Card.values()));

        while (!allExistingCards.isEmpty()) {
            Card randomSelectedCard = allExistingCards.get(random.nextInt(allExistingCards.size()));

            cardsInDeck.add(randomSelectedCard);
            allExistingCards.remove(randomSelectedCard);
        }
    } 

    /*
     * Returns the next card from this deck and removes it.
     * Precondition: The deck isn't empty
     */
    public Card next() {
        Card nextCard = cardsInDeck.get(0);
        cardsInDeck.remove(0);
        return nextCard;
    }
	
	public Card getTrumpCard(){
		return cardsInDeck.get(cardsInDeck.size() - 1);
	}

    /*
     * Returns true if this deck is empty.
     */
    public boolean isEmpty() {
        return (cardsInDeck.isEmpty());
    }


    private class DeckIterator implements Iterator<Card> {
        private Deck deck;
        private int next;

        /*
         * Constructor
         */
        private DeckIterator(Deck deck) {
            this.deck = deck;
            next = 0;
        }

        /*
         * Returns true if there is a next card in this deck.
         */
        public boolean hasNext() {
            return (next < deck.cardsInDeck.size());
        }

        /*
         * Returns the next card of this deck.
         */
        public Card next() {
            if (!hasNext())
                throw new NoSuchElementException("Elementet findes ikke");
            Card card = deck.cardsInDeck.get(next);
            next++;
            return card;
        }
    }

    public Iterator<Card> iterator() {
        return new DeckIterator(this);
    }

    public String toString() {
        String string = "";
        for (Card card : cardsInDeck) {
            string += card.toString() + " ";
        }
        return string;
    }

    public boolean equals(Object other) {
        if (this == other)
            return true;
        
        if (!(other instanceof Deck))
            return false;


        if (other == null)
            return false;

        Deck otherDeck = (Deck)other;
        boolean equals = true;
        
        if (otherDeck.cardsInDeck.size() != cardsInDeck.size()) {
            equals = false;
        }
        
        int i = 0;
        while (equals && i < cardsInDeck.size()) {
            if (!cardsInDeck.get(i).equals(otherDeck.cardsInDeck.get(i)))
                equals = false;
            i++;
        }

        return equals;
    }
}