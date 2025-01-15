#ifndef evaluate_h
#define evaluate_h
#include "deck.h"

int check_flush(Card* hand[HAND_SIZE]);

// check for straight
int check_straight(Card* hand[HAND_SIZE]);

// check for pair / three / four

int* check_multi(Card* hand[HAND_SIZE]);

int check_highest(Card* hand[HAND_SIZE]);

// evaluate hand checks to see what score a hand will get
int evaluate_hand(Card* hand[HAND_SIZE]); 
#endif
