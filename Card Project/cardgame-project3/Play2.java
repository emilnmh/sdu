import java.util.Scanner;

public class Play2 {
    private static Scanner input = new Scanner(System.in);

    /*
     * Main program
     */
    public static void main(String[] args) {

        System.out.println("Starting...");
        String playerName = setPlayerName();
        Player player1 = new Player(playerName);

        AutoPlayer player2 = new AutoPlayer("giga nerd");

        Board board = new Board(player1, player2);
        
		player2.setTrumpCard(board.trumpCard());
		
        System.out.println("Trumpkortet: " + board.trumpCard()); 
 
        // Game Loop, every iteration is a round.
        while (!board.gameOver()) { 
            System.out.println("\n\n*New round*\n\n");

            Card firstPlayerSelectedCard;
            Card secondPlayerSelectedCard;
              
            if (board.next().equals(player1)) { // If player1 goes first
                // First player makes a move
                System.out.println(player1.name() + "'s turn. Selected a card.");
                displayHand(player1);
                firstPlayerSelectedCard = playerSelectCard(player1);
                System.out.println(player1.name() + " played:\n\t" + firstPlayerSelectedCard.toString() + "\n");
                
                // Second player makes a move
                System.out.println(player2.name() + "'s turn. Selected a card.");
				displayHand(player2);
                secondPlayerSelectedCard = player2.next(firstPlayerSelectedCard);
                System.out.println(player2.name() + " played:\n\t" + secondPlayerSelectedCard.toString() + "\n");
            } else { // If player2 goes first
                // First player makes a move
                System.out.println(player2.name() + "'s turn. Selected a card.");
				displayHand(player2);
                firstPlayerSelectedCard = player2.next();
                System.out.println(player2.name() + " played:\n\t" + firstPlayerSelectedCard.toString() + "\n");

                // Second player makes a move
                System.out.println(player1.name() + "'s turn. Selected a card.");
                displayHand(player1);
                secondPlayerSelectedCard = playerSelectCard(player1);
                System.out.println(player1.name() + " played:\n\t" + secondPlayerSelectedCard.toString() + "\n");
            }

            System.out.println(board.move(firstPlayerSelectedCard, secondPlayerSelectedCard) + " won the round.");
        }

        // Game Over
        System.out.println("\n" + "--- Game over ---" + "\n");

        int player1Points = getPoints(player1);
        int player2Points = getPoints(player2);
        System.out.println(player1.name() + ": " + player1Points + " Points");
        System.out.println(player2.name() + ": " + player2Points + " Points");
        
        //Prints the winner.
		if (player1Points > player2Points)
			System.out.println("The winner is " + player1.name());
		else if (player1Points < player2Points)
            System.out.println("The winner is " + player2.name());
        else {
            System.out.println("It's a Tie.");
        }
    }

    /*
     * Displays a player's hand.
     */
    private static void displayHand(Player player) {
        Card[] playersHand = player.hand();

        System.out.println("Your hand:");
        for (int i = 0; i < player.cardsInHand(); i++) {
            System.out.println((i+1) + ": " + playersHand[i]);
        }
		
		
		
    }

    /*
     * The player select which card to use.
     */
    private static Card playerSelectCard(Player player) {
        int cardIndexInHand = 0; // 0 is the not assigned value.
        do {
            System.out.println("Choose the card you want to use. Type 'h' or 'help' for help");
            String inputFromUser = input.nextLine();
            if (inputFromUser.equalsIgnoreCase("h") || inputFromUser.equalsIgnoreCase("help")) {
                System.out.println("Type the number infront of the card to use it. \nType 'hand' to display the hand.");
            } else if (inputFromUser.equalsIgnoreCase("hand")) {
                displayHand(player);
            } else if (inputFromUser.equals("1") || inputFromUser.equals("2") || inputFromUser.equals("3")) {
                cardIndexInHand = Integer.parseInt(inputFromUser);
            } 
        } while (cardIndexInHand <= 0 || cardIndexInHand > player.cardsInHand());

        return player.hand()[cardIndexInHand - 1];
    }


    /*
     * Gets a name from the user, and returns it.
     */
    private static String setPlayerName() {
        String name = "";
        do {
            System.out.print("Name your player: ");
            name = input.nextLine();
        } while (name == "");
        return name;
    }

    /*
     * Returns the amount of point in player's collection pile.
     */
    private static int getPoints(Player player) {
        Card[] cardsCollected = player.collectedCards();
        int points = 0;
        for (Card card : cardsCollected) {
            points = points + card.points();
        }
        return points;
    }
}
