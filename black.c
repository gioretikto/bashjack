#include "black.h"
#include <stdio.h>
#include <ctype.h>

#define SCREEN_LINE_COUNT 25

enum outcomes {NO_WINNER, PLY_WINS, BLACKJACK, CPU_WINS, TIE};

extern struct card deck[CARDS];

void init_game(struct black *table) {

	table->player[PLY].total = table->player[CPU].total = 0;
	table->player[PLY].hand = table->player[CPU].hand = 0;
	table->player[PLY].aces = table->player[CPU].aces = 0;
	table->check_stand = 0;
	table->end = 0;
	
	bet(table);
	
			
	if (table->cards_dealt == 0 || table->cards_dealt > CARDS-14)
	{
		shuffle(deck);
		table->cards_dealt = 0;
	}
	
	/* Deal 1st hand: first two cards */
	
	unsigned int i;
	
	for (i = 0; i < 2; i++)
	{
		getCard(table, CPU);
		getCard(table, PLY);
	}
	
	findWinner(table);
}

void assignPoints(struct player_data *player) {

		if (player->card_value == 11)
			player->aces++;
			
		if ( (player->aces > 0) && ( (player->total + player->card_value) > 21) ) {
		
			player->total += player->card_value;
			
			player->total = player->total - 10;		/* Ace now counts as 1 */
			
			player->aces--;
		}
		
		else
			player->total += player->card_value;		
}

int findWinner (struct black *table) {

	enum outcomes winner;

	if (table->check_stand == 1) {
	
		if (table->player[CPU].total == table->player[PLY].total)			/* Tie */
			winner = TIE;
	
		else if (table->player[CPU].total <= 21) {
		
			if (table->player[PLY].total > table->player[CPU].total)  	/* Player wins */
				winner = PLY_WINS;
			else
				winner = CPU_WINS;
		}
			
		else   													/* Dealer wins  player->total > 21 */
			winner = CPU_WINS;
	}
	
	else {
	
		if ( (table->player[PLY].total == 21) && (table->player[CPU].total == 21) )		/* A tie */
			winner = TIE;
	
		else if (table->player[PLY].total > 21) 		/* If the player exceeds a sum of 21 "busts", loses, even if the dealer also exceeds 21 */
			winner = CPU_WINS;

		else if (table->player[PLY].total == 21)
				winner = PLY_WINS;
					
		else if (table->player[CPU].total == 21)		/* Dealer wins */
			winner = CPU_WINS;
			
		else if (table->player[CPU].total > 21)
				winner = PLY_WINS;
			
		else
			winner = NO_WINNER;					/* no winner found */
	}
	
	if (winner != NO_WINNER) {
	
		table->end = 1;

		if (winner == BLACKJACK) {
			table->credit += (table->bet * 3)/2;
			printf("Congratulation you win: \n\n");			
		}
		
		else if (winner == PLY_WINS) {
			table->credit += table->bet;
			printf("Congratulation you win: \n\n");
		}
					
		else if (winner == CPU_WINS) {
			table->credit -= table->bet;
			printf("Dealer wins \n\n");
		}
					
		else
			printf("No one wins\n");
		
		if (table->credit <= 0) {
			table->credit = 50;
			printf("Dealer wins. Sorry you bankrupted\n\n");				
		}
		
		printCards(table);
		printf("\nCredit: %d$\n", table->credit);
		printf("Enter a key to start a new game\n");
		clearScreen();
		
		int ans;		
		do {
			ans = toupper(getchar());
			
			if (ans == 'Q' || ans =='X')
				return 0;				
					
			else 
				break;
				
			ans = toupper(getchar());
				
		} while(1);

		init_game(table);
	}
	
	return 1;
}

void getCard(struct black *table, enum players id)
{
	if (table->player[CPU].hand == 0 && id == CPU)
	{
		table->player[CPU].card[table->player[id].hand] = deck[table->cards_dealt].symbol;
		table->covered_card = deck[table->cards_dealt].symbol;		
	}
	
	else
		table->player[id].card[table->player[id].hand] = deck[table->cards_dealt].symbol;
		
	table->player[id].card_value = deck[table->cards_dealt].value;
	assignPoints(&table->player[id]);
	table->player[id].hand++;
	table->cards_dealt++;
}

void printCards(struct black *table) {

	unsigned int i;
	
	clearScreen();

	for (i = 0; i < table->player[CPU].hand; i++) { 	/* print Dealer's cards */
	
		if (table->end == 0 && i == 0) {
			printf("%s ", "🂠");
			continue;
		}
		
		else
			printf("%s ", table->player[CPU].card[i]);
	}
	
    printf("\n\n");
	
	for (i = 0; i < table->player[PLY].hand; i++) 		/* print Player's cards */		
		printf("%s ", table->player[PLY].card[i]);
}

void bet (struct black *table) {

	printf("What's your bet?\n");
	
	while ( (scanf("%d", &table->bet) != 1) || (table->bet > table->credit) ) {
	    printf ("Enter a valid sum, your credit is %d$:\n", table->credit);
	    getchar();
	}
}

void clearScreen(void)
{
	unsigned int i;
	
	while (i < SCREEN_LINE_COUNT) {
		 printf("\n"); 	/* Clears screen by printing a number of blank lines */
		 i++;
	}
}
