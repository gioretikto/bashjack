#include <stdlib.h>
#include <time.h>

#include "black.h"

struct card	deck[] = {
					{"🂡",11},
					{"🂢",2},
					{"🂣",3},
					{"🂤",4},
					{"🂥",5},
					{"🂦",6},
					{"🂧",7},
					{"🂨",8},
					{"🂩",9},
					{"🂪",10},
					{"🂫",10},
					{"🂭",10},
					{"🂮",10},
					{"🂱",11},
					{"🂲",2},
					{"🂳",3},
					{"🂴",4},
					{"🂵",5},
					{"🂶",6},
					{"🂷",7},
					{"🂸",8},
					{"🂹",9},
					{"🂺",10},
					{"🂻",10},
					{"🂽",10},
					{"🂾",10},
					{"🃁",11},
					{"🃂",2},
					{"🃃",3},
					{"🃄",4},
					{"🃅",5},
					{"🃆",6},
					{"🃇",7},
					{"🃈",8},
					{"🃉",9},
					{"🃊",10},
					{"🃋",10},
					{"🃍",10},
					{"🃎",10},
					{"🃑",11},
					{"🃒",2},
					{"🃓",3},
					{"🃔",4},
					{"🃕",5},
					{"🃖",6},
					{"🃗",7},
					{"🃘",8},
					{"🃙",9},
					{"🃚",10},
					{"🃛",10},
					{"🃝",10},
					{"🃞",10}
};

/* The following is The Fisher–Yates algorithm to shuffle the deck */

void swap (struct card *a, struct card *b) { 
	struct card temp = *a;
    *a = *b; 
    *b = temp; 
}

static int randgen(int lower, int upper) {
  int r;
  int span = upper - lower + 1;
  int cutoff = (RAND_MAX / span) * span;

  while ((r = rand()) >= cutoff)
    continue;

  return ((r % span) + lower);
}
  
void shuffle(struct card *xdeck) {
  for (int i = CARDS - 1; i >= 1; --i)
    swap(&xdeck[i],             	  // swap the current element
         &xdeck[randgen(0, i)]);      // with some random element
}
