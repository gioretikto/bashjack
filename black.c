#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define CARDS 13
#define SUITS 4
#define MAX_CARDS 5
#define SCREENLINECOUNT 25
#define BUF_NAME 15
     					    		
struct table{	  
	const char *player_cards[MAX_CARDS];
    const char *dealer_cards[MAX_CARDS];
    
    char player_name[BUF_NAME]; /* Holds the user's first name. */
    
    int player_points[MAX_CARDS];
    int dealer_points[MAX_CARDS];
    
    int total_player;	/* total player's points */	
    int total_dealer;	/* total dealer's points */
    
    int hand_player;	/* number of cards dealt for player */
    int hand_dealer;	/* number of cards dealt for dealer */
    
    int aces_dealer;	/* These variable are used to attribute values 1/11 to Aces*/
    int aces_player;
    
    int credit, bet;
};

void draw_cards(struct table *game);
void print_cards(struct table *game, char check_winner);
void init(struct table *game);
int findWinner(struct table *game, char check_stand);
void assign_player_points(struct table *game);
void assign_dealer_points(struct table *game);
void bet (struct table *game);

int main()
{	
    struct table game = {0};
	game.credit = 50;
    
    char check_winner; /* to check the presence of a winner */
    char check_stand;  /* to check wheter user entered stand */
    check_stand = check_winner = 0;
    
    int ans; /* For Y/N and Hit/S answers */
	
    while (1) {
    
    	if (game.hand_player == 0) {
    	
    		init(&game);
			draw_cards(&game);
	
			assign_player_points(&game);
			game.hand_player++;
			assign_player_points(&game);
			
			assign_dealer_points(&game);
			game.hand_dealer++;
			assign_dealer_points(&game);

			check_winner = findWinner(&game, check_stand);
				
			if (check_winner != 0) { 	/* if there is a winner */
		
				if (check_winner == 4) 	/* Game Over */
					break;
				
				else
					continue;
			}
		}

		printf("Hit or stand (H/S)? ");
				
		do {
			ans = toupper(getchar());
			
			if(ans == 'H' || ans == 'S')
				break;
				
			else 
				continue;
			
		} while(1);
					
		if (ans == 'H') {
				
			game.hand_player++;
			assign_player_points(&game);
						
			if (game.total_dealer < 17) {  /* Dealer has to stand at 17 */
				game.hand_dealer++;
				assign_dealer_points(&game);
			}
		}
				
		else {   /* ans == 'S' */
			
			if (game.total_dealer >= 17)
				check_stand = 1;				
				
			else { 		/* user enters S but dealer's point < 17 */
				game.hand_dealer++;
				assign_dealer_points(&game);
			}
		}
		
		check_winner = findWinner(&game, check_stand);
		
		if (check_winner != 0) {
		
			if (check_winner == 4) /* Game Over */
				break;
					
			else {
				check_stand = 0;
				continue;
			}
		}
  } /* end of while(1) */
 	
    return 0;
}

void draw_cards(struct table *game) {

	const char *deck[SUITS][CARDS] = {{"🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂭","🂮"},
									  {"🂱","🂲","🂳","🂴","🂵","🂶","🂷","🂸","🂹","🂺","🂻","🂽","🂾"},
		     						  {"🃁","🃂","🃃","🃄","🃅","🃆","🃇","🃈","🃉","🃊","🃋","🃍","🃎"},
        					    	  {"🃑","🃒","🃓","🃔","🃕","🃖","🃗","🃘","🃙","🃚","🃛","🃝","🃞"}};
    int card_pick, suit_pick;
    int i = 0;
      
    int drawn_card[SUITS][CARDS] = {0};
    
    srand((unsigned)time(NULL));
    
    while (i < MAX_CARDS) { 					/* draw dealer's cards */
    
    	suit_pick = rand() % SUITS;
    	card_pick = rand() % CARDS;
    	
    	if (drawn_card[suit_pick][card_pick] == 0) {
    	
	    	game->dealer_cards[i] = deck[suit_pick][card_pick];
	    	drawn_card[suit_pick][card_pick] = 1;
			
			if (card_pick == 0) {				/* Here points are defined but not still assigned */
				game->dealer_points[i] = 11;
				game->aces_dealer++;
			}
			
			else if (card_pick >= 9)
				game->dealer_points[i] = 10;
					
			else
				 game->dealer_points[i] = card_pick + 1;
				 
			i++;
		}
	}
		
	i = 0;
	
    while (i < MAX_CARDS) { 					/* draw player's cards */
    
    	card_pick = rand() % CARDS;
    	suit_pick = rand() % SUITS;
    	
		if (drawn_card[suit_pick][card_pick] == 0) {
    	
	    	game->player_cards[i] = deck[suit_pick][card_pick];
	    	drawn_card[suit_pick][card_pick] = 1;
			
			if (card_pick == 0) {				/* Here points are defined but not still assigned */
				game->player_points[i] = 11;
				game->aces_player++;
			}
			
			else if (card_pick >= 9)
				game->player_points[i] = 10;
					
			else
				 game->player_points[i] = card_pick + 1;
			
	    	i++;
		}
	}
		
	return;
}

void assign_player_points(struct table *game) {

	game->total_player += game->player_points[game->hand_player];
	
	if(game->aces_dealer > 0 && game->total_player > 21) {
			
				game->total_player = game->total_player - 10;
				game->aces_player--;
	}		
		
	return; 
}

void assign_dealer_points(struct table *game) {

	game->total_dealer += game->dealer_points[game->hand_dealer];
	
	if(game->aces_dealer > 0 && game->total_player > 21) {
			
		game->total_dealer = game->total_dealer - 10;
		game->aces_dealer--;
	}
	
	return;
}

void print_cards(struct table *game, char check_winner) {

	int i;
	
	printf("Dealer's cards:\n\n");
	
	if (check_winner != 0)
		printf("%s ", game->dealer_cards[0]);
	else	
		printf("%s ", "🂠");
	
	for (i = 0; i <= game->hand_dealer; i++) { /* print dealer's cards */
	
		if (i == 0)
			continue;
		else 
		   	printf("%s ", game->dealer_cards[i]);
	}
		
	printf("\n\n");
	
	printf("Your cards:\n\n");
	
	for (i = 0; i <= game->hand_player; i++) /* print player's cards */
    	printf("%s ", game->player_cards[i]);
    	
    while (i < 17) {
   		 printf("\n"); /* Clears screen by printing a number of blank lines. */
   		 i++;
  	}
    return;
}

void init(struct table *game) {

	game->total_player = 0;
	game->total_dealer = 0;
	
	printf("\n\nStep right up to the Blackjack tables\n\n");

	if (game->player_name[0] == 0) {
		printf("\nWhat is your first name? ");
		
		do {
			fgets(game->player_name, BUF_NAME, stdin);
			
			if(!isprint(game->player_name[0])) {
				printf("please enter a valid name\n");
				continue;
			}
			
			else
				break;
			
		} while (1);
			
		printf("Ok, %sGet ready for casino action * Blackjack pays 3 to 2 * You're credit is 50$ * Press enter to start\n\n", game->player_name);
		bet(game);
	}
	
	return;
}

int findWinner (struct table *game, char check_stand) {

	char winner, ans;

	if (check_stand == 1) {
	
		if (game->total_dealer == game->total_player)							/* Tie */
			winner = 5;
	
		else if ( (game->total_player <= 21) && (game->total_player > 17) )  	/* Player wins */
			winner = 2;		
		
		else   /* Dealer wins */
			winner = 3;
	}
	
	else {

		if ( (game->total_player == 21) && (game->total_dealer == 21) )			/* A tie */
			winner = 5;

		else if ( (game->total_player == 21) && (game->total_dealer != 21) )	/* Player wins with Blackjack */
			winner = 1;
		
		else if ( (game->total_player <= 21) &&  (game->total_dealer > 21) )	/* Player wins */
			winner = 2;
					
		else if ( (game->total_dealer == 21) && (game->total_player != 21) )	/* Dealer wins */
			winner = 3;
			
		else if ( (game->total_dealer < 21) && (game->total_player > 21) )		/* Dealer wins */
			winner = 3;
		
		else
			winner = 0;		/* no winner found */
	}
	
	if (winner > 0) {
		
		print_cards(game, winner);
		
		game->hand_player = game->hand_dealer = 0; 		/* zero number of cards dealt */
		game->aces_player = game->aces_dealer = 0; 		/* zero number of aces */
		
		if (winner == 1) {
			printf("Congratulation you win!\n");
			game->credit += (game->bet * 3)/2;
		}
		
		else if (winner == 2) {
			printf("Congratulation you win!\n");
			game->credit += game->bet;
		}			
					
		else if (winner == 3) {
			printf("Sorry, Dealer wins\n");
			game->credit -= game->bet;
		}
					
		else
			printf("No one wins\n");
		
		printf("credit: %d$\n", game->credit);
		
		if (game->credit <= 0) {
			printf("Sorry you bankrupted\n");
			return 4;				/* Game Over */	
		}
		
		else {
			printf("Play again (Y/N)?\n");
			
			do {
				ans = toupper(getchar());
				
				if(ans == 'Y') {
					bet(game);
					return 1;
				}
				
				else if (ans == 'N') {
					printf("Goodbye!\n");
					return 4;		/* Game Over */					
				}
				
				else
					continue;
				} while(1);
		 }
	}
	
	print_cards(game, winner);
	
	return 0;
}

void bet (struct table *game) {

	printf("What's your bet?\n");
	
	while ( (scanf("%d", &game->bet) != 1) || game->bet > game->credit)
  	{
    	while (getchar() != '\n');
    	printf ("Enter a valid sum, your credit is %d$:\n",game->credit);
  	}
	
	return;
}