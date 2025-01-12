#include<stdio.h>
#include<stdlib.h>

#define DECK_SIZE 52 // defines all sizes so no hard-coding
#define NUM_SUITS 4
#define NUM_VALUES 13
#define POCKET_SIZE 2
#define HAND_SIZE 5
#define FLOP_SIZE 3
#define TURN_SIZE 1
#define RIVER_SIZE 1


#ifndef assert 
#define assert(arg)     (arg ? 0 : (printf("Assertion %s failed\n", #arg), exit(-1)))
#endif

typedef enum {Clubs, Diamonds, Spades, Hearts} Suit; // mostly for comparing hands
typedef enum {Ace, Joker=11, Queen, King} Value;

typedef struct card // card contains a suit and a value 
{
  Suit suit;
  Value value;
} Card;

typedef struct deck // deck contains a list of all cards and a index so we can mimic a stack without actually making one
{
  Card* card_list[DECK_SIZE];
  int index;
} Deck;

typedef struct player // a player has pocket cards and their final hand to be scored
{
  Card* pocket[POCKET_SIZE];
  Card* hand[HAND_SIZE];
} Player;

// --DECK FUNCTIONS--

// initialize_deck fills the deck with every value of card
void initialize_deck(Deck* deck) { 
  deck->index = 0; // set the index to the top of the stack
  for (int i = 0; i < DECK_SIZE; i++)
  {
    deck->card_list[i] = (Card*) malloc(sizeof(Card)); // adds each value suit pair to the stack
    deck->card_list[i]->suit = i % NUM_SUITS; 
    deck->card_list[i]->value = i % NUM_VALUES;
  }
}

// shuffle_deck randomizes the deck by swapping each index with a random index
void shuffle_deck(Deck* deck) {
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

// deal_cards fills a players pocket with two cards
void deal_cards(Player* player, Deck* deck) {
  for (int i = 0; i < POCKET_SIZE; i++)
  {
    player->pocket[i] = deck->card_list[deck->index]; // fills each pocket slot with a card
    deck->index++; // make sure to increment the index after each card is dealt
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
  Card* cards = (Card*) malloc(FLOP_SIZE * sizeof(Card));
  burn_card(deck);
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

//TODO destroy deck

void play_hand() {
  Deck* deck = (Deck*) malloc(sizeof(Deck)); //create deck 
  Player* player = (Player*) malloc(sizeof(Player)); // create two players, player & computer
  Player* computer = (Player*) malloc(sizeof(Player));
  initialize_deck(deck); // create the deck
  shuffle_deck(deck); // shuffle
  
  deal_cards(player, deck); // give both player cards
  deal_cards(computer, deck); 
  
  Card* table = (Card*) malloc(sizeof(Card) * FLOP_SIZE); // flop
  table = deal_flop(deck);

  table = (Card*) realloc(table, sizeof(Card) * (FLOP_SIZE + TURN_SIZE)); // turn
  table[3] = *deal_turn(deck);

  table = (Card*) realloc(table, sizeof(Card) * (FLOP_SIZE + TURN_SIZE + RIVER_SIZE)); // river
  table[4] = *deal_river(deck);
}

int main() {
    play_hand();
    return 0;
}
