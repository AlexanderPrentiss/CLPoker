#include<stdlib.h>
#include<time.h>
#include "deck.h"

// initialize_deck fills the deck with every value of card
void initialize_deck(Deck* deck) { 
  deck->index = 0; // set the index to the top of the stack
  for (int i = 0; i < DECK_SIZE; i++)
  {
    deck->card_list[i] = (Card*) malloc(sizeof(Card)); // adds each value suit pair to the stack
    deck->card_list[i]->suit = i % NUM_SUITS; 
    deck->card_list[i]->value = 2 + i % NUM_VALUES;
  }
}

// shuffle_deck randomizes the deck by swapping each index with a random index
void shuffle_deck(Deck* deck) {
  srand(time(NULL)); // Sets a random seed using time
  deck->index = 0; // When we shuffle we want to go back to using the top of the card_list
  Card* temp = (Card*) malloc(sizeof(Card));
  int random_index;
  for (int i = 0; i <  DECK_SIZE; i++) // loop through every index
  {
    random_index = rand() % (DECK_SIZE); // find a random index
    temp = deck->card_list[i]; // save the value of the current index
    deck->card_list[i] = deck->card_list[random_index]; // set the value of the current index to the value of the random index
    deck->card_list[random_index] = temp; // set the value of the random index to the saved current index

  }
}

// burn_cards gets rid of the top card by incrementing the index
void burn_card(Deck* deck) {
  deck->index++;    // I don't know if I should keep this function or not. 
                    // I guess it makes the code readable if you know poker lol
}

// deal_flop burns an initial card, and returns the flop (top three cards from the stack)
// most likely use is for the table cards
Card* deal_flop(Deck* deck) {
  Card* cards = (Card*) malloc(FLOP_SIZE * sizeof(Card)); // create an array for the flop
  burn_card(deck); // burn a card
  for (int i = 0; i < FLOP_SIZE; i++)
  {
    cards[i] = *deck->card_list[deck->index]; // adds next card to the list then increments the index
    deck->index++;
  }
  return cards;
}

// deal_turn is same as the flop except onlt one card is drawn
Card* deal_turn(Deck* deck) {
  Card* turn = (Card*) malloc(sizeof(Card));
  burn_card(deck);
  turn = deck->card_list[deck->index];
  deck->index++;
  return turn;
}

// deal_river is the same as deal_turn, just for readablity
Card* deal_river(Deck* deck) {
  Card* river = (Card*) malloc(sizeof(Card));
  burn_card(deck);
  river = deck->card_list[deck->index];
  deck->index++;
  return river;
}

// destroy deck destroys every card in the deck, then destroys the deck
void destroy_deck(Deck* deck) {
  for (int i = 0; i < DECK_SIZE; i++)
  {
    free(deck->card_list[i]);
  }
  free(deck);
}
