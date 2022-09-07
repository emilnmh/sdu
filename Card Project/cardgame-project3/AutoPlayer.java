public class AutoPlayer extends Player {
	private Card trumpCard;
	
    /*
     * Constructor
     */
    public AutoPlayer(String name) {
		super(name);
	}
	
	/*
	 * Setter for trumpCard 
	 */
	public void setTrumpCard(Card trumpCard) {
		this.trumpCard = trumpCard;
	}


	/*
	 * Getter for trumpCard
	 */
	public Card getTrumpCard() {
		return trumpCard;
	}

    /*
     * Returns the next move by this autoplayer,
     * in case it is the first to play this round.
     */
    public Card next() { 
		Card[] hand = hand();
		Card bestCard = hand[0];
		
		//Choose card with highest value
		for(int i = 0; i < cardsInHand(); i++){
			if(hand[i].isHigherThan(bestCard))
				bestCard = hand[i];
		}
		
		//if current best card is trump suit choose a card in another suit
		if(!(trumpCard == null) && bestCard.suit.equals(trumpCard.suit)){
			for(int i = 0; i < cardsInHand(); i++){
				if(!hand[i].suit.equals(trumpCard.suit))
					bestCard = hand[i];
			}
		}
		
		return bestCard;
    }

    /*
     * Returns the next move by this autoplayer,
     * in case it is to play second this round.
     */
    public Card next(Card otherCard) { 
        Card[] hand = hand();
		Card bestCard = hand[0];
		boolean canWin = false;
		
		
		// Checks if any cards in hand can win against otherCard and chooses the winning card with the highest point value if there is more than one winning card.
		for(int i = 0; i < cardsInHand(); i++){
			if(hand[i].suit.equals(otherCard.suit) && hand[i].isHigherThan(otherCard) && (hand[i].points() > bestCard.points() || !canWin) ){ 
				bestCard = hand[i];
				canWin = true;
			}
		}

		
		
		// Checks if bot can win by using a card in trump suit.
		if(!(trumpCard == null) && !canWin && !otherCard.suit.equals(trumpCard.suit)){
			for(int i = 0; i < cardsInHand(); i++){
				if(hand[i].suit.equals(trumpCard.suit) && (hand[i].points() > bestCard.points() || !canWin)){
					bestCard = hand[i];
					canWin = true;
				}
			}
				
		}
		
		
		// If from previous two loops no winning card was found choose the card with lowest value.
		if(!canWin){
			for(int i = 0; i < cardsInHand(); i++){
				if(hand[i].points() < bestCard.points())
					bestCard = hand[i];
			}
		}
		
		
		
        return bestCard;
	}

	public String toString() {
		return name() + " trumpCard: " + trumpCard.toString();
	}

	public boolean equals(Object other) {
		return (this == other);
	}
}