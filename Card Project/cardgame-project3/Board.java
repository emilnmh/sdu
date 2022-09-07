public class Board {
    private Player player1;
    private Player player2;
    private Card trumpCard;
    private Player nextPlayer;
	private Deck deck;


    /*
     * Constructor
     */
    public Board(Player player1, Player player2) {
		this.deck = new Deck();
        this.player1 = player1;
        this.player2 = player2;
        this.trumpCard = deck.getTrumpCard();
        this.nextPlayer = player1;
        
        //Add cards to players hand
        for (int i = 0; i < 3; i++) {
            player1.addToHand(deck.next());
            player2.addToHand(deck.next());
        }
    }

    /*
     * Returns the player who starts the next round.
     */
    public Player next() {
        return this.nextPlayer;
    }

    /*
     * Getter method for trumpCard
     */
    public Card trumpCard() {
        return this.trumpCard;
    }


    /*
     * Returns true if the game is finished 
     */
	public boolean gameOver() {
		return (deck.isEmpty() && player1.cardsInHand() == 0 && player2.cardsInHand() == 0);
	}
	
	
    /*
     * Updates this board after the first player to play in this round card1 
     * and the other player to play card2.
     * Returns a reference to player who won the round. 
     */
    public Player move(Card card1, Card card2) {
        // Find the winner of the round
        Player winningPlayer;
        if (card2.suit.equals(card1.suit) && card2.isHigherThan(card1) || card2.suit.equals(trumpCard.suit) && !card1.suit.equals(trumpCard.suit)) { // not nextPlayer won (card 2 won)
            if (nextPlayer.equals(player1)) { // player2 won
                winningPlayer = player2;
            } else { // player1 won
                winningPlayer = player1;
            }
        } else { // nextPlayer won (card1 won)
            winningPlayer = nextPlayer;
        }
		
		//Removes the card which was played from each player's hand
		if (nextPlayer.equals(player1)){
			player1.removeFromHand(card1);
			player2.removeFromHand(card2);
		}
		else{
			player1.removeFromHand(card2);
			player2.removeFromHand(card1);
			
		}
		
		
		//Adds cards to the winning player's hand
        winningPlayer.addToCollectedCards(card1);
        winningPlayer.addToCollectedCards(card2);

        
        //Adds a new card to each player's hand
		if (!deck.isEmpty())
            player1.addToHand(deck.next());
        
        if(!deck.isEmpty())
            player2.addToHand(deck.next());
			

        // set nextPlayer to winningPlayer
        nextPlayer = winningPlayer;
		
        return winningPlayer;
    }

    public String toString() {
        String string = "";
        string += "Player1: " + player1.toString() + " ";
        Card[] player1Cards = player1.hand();
        for (Card card : player1Cards) {
            string += card.toString() + " ";
        }

        string += "Player2: " + player2.toString() + " ";
        Card[] player2Cards = player2.hand();
        for (Card card : player2Cards) {
            string += card.toString() + " ";
        }

        string += "Nextplayer: " + nextPlayer.toString() + " ";
        string += "Trumpcard: " + trumpCard.toString() + " ";

        string += "Deck: " + deck.toString(); 
        return string;
    }

    public boolean equals(Object other) {
        if (this == other)
            return true;

        if (!(other instanceof Board))
            return false;

        if (other == null) {
            return false;
        }

        Board otherBoard = (Board)other;
        return (player1.equals(otherBoard.player1) && player2.equals(otherBoard.player2) && deck.equals(otherBoard.deck) && trumpCard.equals(otherBoard.trumpCard) && nextPlayer.equals(otherBoard.nextPlayer));
    }
}




