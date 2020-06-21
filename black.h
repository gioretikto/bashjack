#define CARDS 52
#define MAX_CARDS_HAND 7

enum players {CPU,PLY};

struct player_data {

	unsigned int card_value;  	
	unsigned int total;			/* total points */
	unsigned int hand;			/* card box index */
  	unsigned int aces; 			/* number of aces */
  	const char *card[MAX_CARDS_HAND];

};

struct black {

	int credit;
	int bet;
	
	unsigned int cards_dealt;
	
	const char *covered_card;
	
	struct player_data player[2];
	
	_Bool check_stand;
	_Bool end;	

};

struct card {

   	const char *symbol;
   	unsigned int value;   	
   	
};

int bet (struct black *table);
void shuffle (struct card *xdeck);
void swap (struct card *a, struct card *b);
void assignPoints(struct player_data *player);
void getCard(struct black *table, enum players id);
int findWinner (struct black *table);
void printCards(struct black *table);
int init_game(struct black *table);
void clearScreen(void);
