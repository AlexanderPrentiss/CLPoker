#ifndef deck_h
#define deck_h
#include "preprocess.h"

typedef struct card // card contains a suit and a value 
{
  int suit;
  int value;
} Card;

typedef struct deck // deck contains a list of all cards and a index so we can mimic a stack without actually making one
{
  Card* card_list[DECK_SIZE];
  int index;
} Deck;

// initialize_deck fills the deck with every value of card
void initialize_deck(Deck* deck); 

// shuffle_deck randomizes the deck by swapping each index with a random index
void shuffle_deck(Deck* deck);

// burn_cards gets rid of the top card by incrementing the index
void burn_card(Deck* deck);

// most likely use is for the table cards
Card* deal_flop(Deck* deck);

// deal_turn is same as the flop except onlt one card is drawn
Card* deal_turn(Deck* deck);

// deal_river is the same as deal_turn, just for readablity
Card* deal_river(Deck* deck);

// destroy deck destroys every card in the deck, then destroys the deck
void destroy_deck(Deck* deck);

#endif
