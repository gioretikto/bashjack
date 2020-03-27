#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define ACE 0
#define CARDS 13
#define SUITS 4
#define MAX_CARDS 10
#define SCREENLINECOUNT 25

static const char *deck[SUITS][CARDS] = {{"🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂭","🂮"},
									 	 {"🂱","🂲","🂳","🂴","🂵","🂶","🂷","🂸","🂹","🂺","🂻","🂽","🂾"},
		     							 {"🃁","🃂","🃃","🃄","🃅","🃆","🃇","🃈","🃉","🃊","🃋","🃍","🃎"},
        					    		 {"🃑","🃒","🃓","🃔","🃕","🃖","🃗","🃘","🃙","🃚","🃛","🃝","🃞"}};
        					    		 
enum outcomes {NO_WINNER, PLAYER_WINS, BLACKJACK, DEALER_WINS, TIE, GAME_OVER, ID_DEALER, ID_PLAYER};
       					    			    		
struct table {

	int id;

	int cards[MAX_CARDS];
    
    int points[MAX_CARDS];  /* Points for each hand */
   
    int total;				/* total points */
        
    int hand;				/* number of cards dealt */
    
    int aces;				/* N° of aces: These variable are used to attribute values 1/11 to Aces*/
    
    int credit;
    
    int bet;
};

void draw_cards(struct table *player);
void print_cards(struct table *player, char check_winner);
void init_game(struct table *player);
int findWinner(struct table *dealer, struct table *player, char check_stand);
void assign_points(struct table *player);
void bet (struct table *player);

int main()
{	
    srand((unsigned)time(NULL));
    
    struct table player;
    struct table dealer;
    
   	dealer.id = ID_DEALER;
	player.id = ID_PLAYER;

    char ans = 'M'; /* For Hit/S answers */
    
    char check_winner;  /* to check the presence of a winner */
    char check_stand;   /* to check wheter user entered stand */
   
    check_stand = 0;
	check_winner = 0;
	    
    printf("\n\nStep right up to the Blackjack tables\n\n");
		
	printf("Ok, Get ready for casino action * Blackjack pays 3 to 2 * You're credit is 50$ * Press enter to start\n\n");
	
	init_game(&dealer);
	
	init_game(&player);
	
    for (;;) {
		
		print_cards(&dealer, check_winner);
		print_cards(&player, check_winner);
		
		check_winner = findWinner(&dealer, &player, check_stand);
		
		if (check_winner != NO_WINNER) {
		
			if (check_winner == GAME_OVER) /* Game Over */
				break;
					
			else {
			   	check_stand = 0;
				check_winner = 0;
				init_game(&dealer);
				init_game(&player);
				ans = 'M';
				continue;
			}
		}
		
		else {
		
			printf("Hit or stand (H/S)? ");
					
			do {
				ans = toupper(getchar());
				
				if (ans == 'H' || ans == 'S')
					break;
					
				else 
					continue;
				
			} while(1);
			
			if (ans == 'H') {
			
				if (dealer.total < 17) {
					assign_points(&player);
					assign_points(&dealer);
				}
				
				else 
					assign_points(&player);
			}
			
			if (ans == 'S') {
				if (dealer.total < 17)
					assign_points(&dealer);
				else
					check_stand = 1;
			}
		}
			
  } /* end of for(;;) */

    return 0;
}

void init_game(struct table *player) {

	player->total = 0;

	player->aces = 0;
	
	player->hand = 0;
	
	draw_cards(player);
	
	assign_points(player);
			
	assign_points(player);
	
	if (player->id == ID_PLAYER) {
		player->credit = 50;
		bet(player);
	}
}

void draw_cards(struct table *player) {

    int card_pick, suit_pick;
    int i = 0;
      
    int drawn_card[SUITS][CARDS] = {0};
    
    while (i < MAX_CARDS) { 					/* draw dealer's cards */
    
    	suit_pick = rand() % SUITS;
    	card_pick = rand() % CARDS;
    	
    	if (drawn_card[suit_pick][card_pick] == 0) {
    	
	    	player->cards[i] = suit_pick * CARDS + card_pick;
	    	drawn_card[suit_pick][card_pick] = 1;
			
			if (card_pick == ACE)
				player->points[i] = 11;
			
			else if (card_pick >= 9)
				player->points[i] = 10;
					
			else
				 player->points[i] = card_pick + 1;
				 
			i++;
		}
	}
		
	return;
}

void assign_points(struct table *player) {

		if (player->points[player->hand] == 11)
			player->aces++;
			
		if ( (player->aces > 0) && ( (player->total + player->points[player->hand]) > 21) ) {
			player->total += player->points[player->hand];
			
			player->total = player->total - 10;
			player->aces--;
		}
		
		else
			player->total += player->points[player->hand];
			
		player->hand++;
}

void print_cards(struct table *player, char check_winner) {

	int i;
	int suit_pick, card_pick;
	
	if (player->id == ID_DEALER)
		printf("Dealer's cards:\n\n");
	else
		printf("Your cards:\n\n");
	
	if (player->id == ID_DEALER) {
	
		if (check_winner != NO_WINNER) {  	/* Show dealer's first card at end of game */
			suit_pick = player->cards[0] / CARDS;
			card_pick = player->cards[0] % CARDS;
			printf("%s ", deck[suit_pick][card_pick]);
		}
		
		else
			printf("%s ", "🂠");
	}
	
	for (i = 0; i < player->hand; i++) { 	/* print player's cards */
	
		if (i == 0 && player->id == ID_DEALER)	/* First dealer's card is coveres */
			continue;
		else {
			suit_pick = player->cards[i] / CARDS;
			card_pick = player->cards[i] % CARDS;
		   	printf("%s ", deck[suit_pick][card_pick]);
		}
	}
	
	if (player->id == ID_DEALER)
	    printf("\n\n");
	    
	else {
		while (i < 17) {
	   		 printf("\n"); 	/* Clears screen by printing a number of blank lines */
	   		 i++;
	  	}
	}
}

int findWinner (struct table *dealer, struct table *player, char check_stand) {

	char winner;

	if (check_stand == 1) {
	
		if (dealer->total == player->total)			/* Tie */
			winner = TIE;
	
		else if (player->total <= 21) {
		
			if (player->total > dealer->total)  	/* Player wins */
				winner = PLAYER_WINS;
			else
				winner = DEALER_WINS;
		}
			
		else   										/* Dealer wins  player->total > 21 */
			winner = DEALER_WINS;
	}
	
	else {
	
		if ( (player->total == 21) && (dealer->total == 21) )		/* A tie */
			winner = TIE;
	
		else if (player->total > 21) 	/* If the player exceeds a sum of 21 "busts", loses, even if the dealer also exceeds 21 */
			winner = DEALER_WINS;

		else if (player->total == 21) {
			if (player->hand == 2)		/* Player wins with Blackjack */
				winner = BLACKJACK;
			else
				winner = PLAYER_WINS;
		}
					
		else if (dealer->total == 21)		/* Dealer wins */
			winner = DEALER_WINS;
			
		else if (dealer->total > 21)
				winner = PLAYER_WINS;
			
		else
			winner = NO_WINNER;		/* no winner found */
	}
	
	if (winner != NO_WINNER) {
		
		print_cards(dealer, winner);
		print_cards(player, winner);
		
		if (winner == BLACKJACK) {
			printf("Congratulation you win!\n");
			player->credit += (player->bet * 3)/2;
		}
		
		else if (winner == PLAYER_WINS) {
			printf("Congratulation you win!\n");
			player->credit += player->bet;
		}
					
		else if (winner == DEALER_WINS) {
			printf("Sorry, Dealer wins\n");
			player->credit -= player->bet;
		}
					
		else
			printf("No one wins\n");
		
		printf("credit: %d$\n", player->credit);
		
		if (player->credit <= 0) {
			printf("Sorry you bankrupted\n");
			return GAME_OVER;				/* Game Over */
		}
		
		else {
			char ans;
			printf("Play again (Y/N)?\n");
			
			do {
				ans = toupper(getchar());
				
				if (ans == 'Y') {
					return 1;
				}
				
				else if (ans == 'N') {
					printf("Goodbye!\n");
					return GAME_OVER;		/* Game Over */
				}
				
				else
					continue;
				} while(1);
		 }
	}
		
	return 0;
}

void bet (struct table *player) {

	printf("What's your bet?\n");
	
	while ( (scanf("%d", &player->bet) != 1) || player->bet > player->credit)
  	{
    	while (getchar() != '\n');
    	printf ("Enter a valid sum, your credit is %d$:\n", player->credit);
  	}
}