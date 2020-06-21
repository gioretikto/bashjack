#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "black.h"

int main(void)
{	
    srand ((unsigned) time(NULL));
    
	struct black table;
	
	table.bet = 0;
	table.credit = 50;
	    
    int ans; /* For Hit/S answers */
    
    printf("\n\nStep right up to the Blackjack tables\n\n");
		
	printf("Ok, Get ready for casino action * Blackjack pays 3 to 2 * You're credit is 50$ * Press enter to start, q to quit\n\n");

	if(!init_game(&table))
		return 0;
	
    for (;;) {
    
		printCards(&table);
		
		printf("\n\nHit or stand (H/S)? ");
					
		do {
			ans = toupper(getchar());
				
			if (ans == 'H' || ans == 'S')
				break;
				
			else if (ans == 'Q' || ans =='X')
				return 0;				
					
			else 
				continue;
				
		} while(1);
			
		if (ans == 'H') {
		
			getCard(&table, PLY);
			
			if (table.player[CPU].total < 17)
				getCard(&table, CPU);
		}
			
		if (ans == 'S') {
		
			if (table.player[CPU].total < 17)
				getCard(&table, CPU);
			else
				table.check_stand = 1;				
		}

		if (!findWinner(&table))
			return 0;
	} /* end of for(;;) */

    return 0;
}
