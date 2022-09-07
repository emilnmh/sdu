public class Player {
    private Card[] cardsInHand;
    private String name;
    private Card[] cardsInCollection;
    private int numberOfCardsInCollection;


    /*
     * Constructor
     */
    public Player(String name) {
        this.name = name;
        cardsInHand = new Card[3];
        for (Card card : cardsInHand) {
            card = null;
        }
        cardsInCollection = new Card[40];
        for (Card card : cardsInCollection) {
            card = null;
        }
        numberOfCardsInCollection = 0;
    }

    /*
     * GETTER: name
     */
    public String name() {
        return this.name;
    }

    /*
     * Returns an array of card which is in the player's hand.
     */
    public Card[] hand() {
        Card[] cardsInHandCopy = new Card[cardsInHand()];
        for (int i = 0; i < cardsInHand(); i++) {
            cardsInHandCopy[i] = cardsInHand[i];
        }
        return cardsInHandCopy;
    }

    /*
     * Returns the number of cards in this player's hand.
     */
    public int cardsInHand() {
        int numberOfCardsInHand = 0;
        while (numberOfCardsInHand < cardsInHand.length && cardsInHand[numberOfCardsInHand] != null) {
            numberOfCardsInHand++;
        }
        return numberOfCardsInHand;
    }

    /*
     * Adds card to this player's hand.
     * PRECONDITION: cards in hand is less than 3
     */
    public void addToHand(Card card) {
        int i = 0;
        boolean cardAddedToHand = false;
        while (i < cardsInHand.length && !cardAddedToHand) {
            if (cardsInHand[i] == null) {
                cardsInHand[i] = card;
                cardAddedToHand = true;
            }
            i++;
        }
    }

    /*
     * Adds card to this player's collection cards.
     */
    public void addToCollectedCards(Card card) {
        cardsInCollection[numberOfCardsInCollection] = card;
        numberOfCardsInCollection++;
    }

    /*
     * Removes card from this player's hand.
     */
    public void removeFromHand(Card card) {
        int i = 0;
        boolean cardRemovedFromHand = false;
        while (i < cardsInHand.length) { // Removes the card from cards in hand.
            if (cardsInHand[i] == card && !cardRemovedFromHand) {
                cardsInHand[i] = null;
                cardRemovedFromHand = true;
            } else if (cardRemovedFromHand) { // Swap card posisitions
                swapNeighboringCardsInHand(i);
            }
            i++;
        }
    }

    /*
     * Returns an array containing the cards collected by this player. 
     */
    public Card[] collectedCards() {
        // Return a copy of collection instead.
        Card[] collectionCopy = new Card[numberOfCardsInCollection];
        for (int i = 0; i < numberOfCardsInCollection; i++) {
            collectionCopy[i] = cardsInCollection[i];
        }
        return collectionCopy;
    }


    /*
     * Swap card in "i" position from cardsInHand array with card in "i - 1" position.
     * Precondition: 0 < i < cardsInHand.length - 1
     */
    private void swapNeighboringCardsInHand(int i) {
        Card placeholderCard = cardsInHand[i];
        cardsInHand[i] = cardsInHand[i - 1];
        cardsInHand[i - 1] = placeholderCard;
    }

    
    /*
     * Equals
     */
    public boolean equals(Object other) {
        return (this == other);
    }


    /*
     * Getter for numberOfCardsInCollection
     */
    public int getNumberOfCardsInCollection() {
        return numberOfCardsInCollection;
    }


    /*
     * Setter for name
     */
    public void setName(String newName) {
        this.name = newName;
    }
}