#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define DECK_SIZE 52 // defines all sizes so no hard-coding
#define NUM_SUITS 4
#define NUM_VALUES 13
#define POCKET_SIZE 2
#define HAND_SIZE 5
#define FLOP_SIZE 3
#define TURN_SIZE 1
#define RIVER_SIZE 1
#define LOWEST_CARD 2


#ifndef assert 
#define assert(arg)     (arg ? 0 : (printf("Assertion %s failed\n", #arg), exit(-1)))
#endif

typedef enum {Clubs, Diamonds, Spades, Hearts} Suit; // mostly for comparing hands
typedef enum {Joker=11, Queen, King, Ace} Value; // Ace high will be default
typedef enum {high, pair = 15, two_pair, three_of_a_kind, straight, flush, full_house, four_of_a_kind, straight_flush, royal_flush} Hand;

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

//check for flush
int check_flush(Card* hand[HAND_SIZE])
{
  Suit test = hand[0]->suit; // set the suit to be the first card's suit
  for (int i = 1; i < HAND_SIZE; i++) // loop through every card
  {
    if (hand[i]->suit != test) // if any card is not of the same suit there is no flush
    {
      return 0;
    }
  }
  return 1; // there was a flush because no card deviated
}

// check for straight
int check_straight(Card* hand[HAND_SIZE]) {
  Card* hashmap[NUM_VALUES+LOWEST_CARD]; // create a hashmap with a slot for every value
  for (int i = 0; i < NUM_VALUES+LOWEST_CARD; i++)
  {
    hashmap[i] = NULL; // default every value in the hash map to a null pointer
  }
  for (int i = 0; i < HAND_SIZE; i++)
  {
    hashmap[hand[i]->value] = hand[i]; // for every card slot the card into the hashmap slot of the value
  }
  int start_index = 0;
  for (int i = 0; i < NUM_VALUES+LOWEST_CARD; i++) 
  {
    if (hashmap[i] != NULL) // the first value that is not a null pointer will be out starting point
    {
      start_index = i;
      break;
    }
  }
  for (int i = start_index; i < start_index+HAND_SIZE; i++) // for the next 5 cards after the starting point
  {
    if (hashmap[i] == NULL) // if any value is a null pointer, there is a hole, meaning no straight
    {
      return 0;
    }
  }
  return 1; // there was a straight
}

// check for pair / three / four

int* check_multi(Card* hand[HAND_SIZE]) {
  int* output = (int*) malloc(sizeof(int) * 3); // initialize an output of three value [lowest value, highest value, case value]
  for (int i = 0; i < 3; i++)
  {
    output[i] = 0; // initialize outputs with zeros
  }
  int hashmap[NUM_VALUES + LOWEST_CARD]; // hashmap for all values same as check_straight
  int count = 0; // this will be the case value
  int pair_count = 0; // checking for two pairs
  for (int i = 0; i < NUM_VALUES + LOWEST_CARD; i++)
  {
    hashmap[i] = 0; // initialize hashmap with zeros
  }
  for (int i = 0; i < HAND_SIZE; i++)
  {
    hashmap[hand[i]->value]++; // count the occurance of each value and increment the hash slot
  }
  for (int i = 0; i < NUM_VALUES + LOWEST_CARD; i++)
  {
    if (hashmap[i] > 1) // if a value occurs more than once there is something of interest
    {
      if (hashmap[i] == 2) pair_count++; // if its a pair track that
      if (hashmap[i] > hashmap[output[1]] || (hashmap[i] == hashmap[output[1]] && i > output[1])) { // if the index is a higher value than the current highest value
            output[0] = output[1]; // swap the current highest to the lowest
            output[1] = i; // make the new value the highes
          }
      count += hashmap[i]; // increment the count
    }
  }
  if (pair_count > 1) count = 6; // 2 is a pair, 3 is toak, 4 is foak, 5 is full house, 6 is two pair.
  output[2] = count; // set the output case value to the count
  return output;
}

int check_highest(Card* hand[HAND_SIZE]){
  int highest = hand[0]->value;
  for (int i = 1; i < HAND_SIZE; i++) // loop through hand to find the highest card
  {
    if (hand[i]->value > highest) highest = hand[i]->value;
  }
  return highest;
}

//TODO evaluate hand will check to see what score a hand will get
int evaluate_hand(Card* hand[HAND_SIZE]) { // use the data we get from the check functions to score the hand
  if (check_flush(hand) && check_straight(hand) && check_highest(hand) == 14) return royal_flush; // if there is an ace and a straight flush; royal flush!
  else if (check_flush(hand) && check_straight(hand)) return straight_flush; // if there is a straight and a flush its a straight flush
  else if (check_flush(hand)) return flush; // check for straight
  else if (check_straight(hand)) return straight; // check for flush
  else // if its none of those check for pairs/toak/foak/fullhouse
  {
    switch (check_multi(hand)[2])
    {
      case 0:
        return check_highest(hand);
        break;
      case 2:
        return pair;
        break;
      case 3:
        return three_of_a_kind;
        break;
      case 4:
        return four_of_a_kind;
        break;
      case 5:
        return full_house;
        break;
      case 6:
        return two_pair;
        break;
    }
  }
  return check_highest(hand); // If its nothing above just return the highest card
}

// set_hand sets a players hand as the highest scoring hand by trying every combo
void set_hand(Player* player, Card* table[HAND_SIZE]){
  int highest = 0;
  Card* cards[HAND_SIZE+POCKET_SIZE];
  for (int i = 0; i < HAND_SIZE+POCKET_SIZE; i++)
  {
    if (i < 2) {
      cards[i] = player->pocket[i];
    } else {
      cards[i] = table[i];
    }
  }
  Card* cur_hand[HAND_SIZE];
  for (int a = 0; a < HAND_SIZE; a++) // I hate this code. I will revisit
  {                                     // each loop checks every value that isnt already selected
    cur_hand[0] = cards[a];
    for (int b = 0; b < HAND_SIZE; b++)
    {
      if (b != a)
      {
        cur_hand[1] = cards[b];
        for (int c = 0; c < HAND_SIZE; c++)
        {
          if (c!=b && c!=a)
          {
            cur_hand[2] = cards[c];
            for (int d = 0; d < HAND_SIZE; d++)
            {
              if (d!=c && d!=b && d!=a)
              {
                cur_hand[3] = cards[d];
                for (int e = 0; e < HAND_SIZE; e++)
                {
                  if (e!=d && e!=c && e!= b && e!=a)
                  {
                    cur_hand[4] = cards[e];
                    if (evaluate_hand(cur_hand) > highest)
                    {
                      for (int z = 0; z < HAND_SIZE; z++) // initialize the players hand and make it the highest hand
                      {
                        player->hand[z] = (Card*) malloc(sizeof(Card));
                        player->hand[z] = cur_hand[z];
                      }
                      highest = evaluate_hand(cur_hand);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// compare_hands decides a winner between two players
Player* compare_hands(Player* playerA, Player* playerB)
{
  if (evaluate_hand(playerA->hand) > evaluate_hand(playerB->hand)) return playerA; // check the evaluation of each hand
  else if (evaluate_hand(playerA->hand) < evaluate_hand(playerB->hand)) return playerB;
  else {
    if (check_straight(playerA->hand) || check_flush(playerA->hand)) // if they are the same check if there is a flush or a straight
    {
      if (check_highest(playerA->hand) > check_highest(playerB->hand)) return playerA; // if there is we just need the highest card
      else if (check_highest(playerA->hand) < check_highest(playerB->hand)) return playerB;
      else return NULL;
    }
    if (check_multi(playerA->hand)[1] > check_multi(playerB->hand)[1]) return playerA; // check the high combo then the low combo of a pair...etc
    else if (check_multi(playerA->hand)[1] < check_multi(playerB->hand)[1]) return playerB;
    else { 
      if (check_multi(playerA->hand)[0] > check_multi(playerB->hand)[0]) return playerA;
      else if (check_multi(playerA->hand)[0] < check_multi(playerB->hand)[0]) return playerB;
      else return NULL; // its actually a tie
    }
  }
}


//TODO destroy deck

// Test : This will probably become the play hand function
void test_evaluations() {
  Deck* deck = (Deck*) malloc(sizeof(Deck)); //create deck 
  Player* player = (Player*) malloc(sizeof(Player)); // create player
  Player* computer = (Player*) malloc(sizeof(Player)); // create computer
  initialize_deck(deck); // initialize deck
  shuffle_deck(deck); // shuffle the deck

  deal_cards(player, deck); // deal cards to the player and computer
  deal_cards(computer, deck);

  Card* table[HAND_SIZE]; // deal cards for the table
  Card* flop = deal_flop(deck);
  for (int i = 0; i < FLOP_SIZE; i++)
  {
    table[i] = &flop[i];
  }
  table[3] = deal_turn(deck);
  table[4] = deal_river(deck);
  
  for (int i = 0; i < HAND_SIZE; i++)
  {
    printf("Table: %d, %d\n", table[i]->suit, table[i]->value); // read values from the table
  }
  
  set_hand(player, table); // find the best hand for the player and the computer
  set_hand(computer, table);
  for (int i = 0; i < HAND_SIZE; i++)
  {
    printf("Player Hand: %d, %d\n", player->hand[i]->suit, player->hand[i]->value); // read player hand
  }
  for (int i = 0; i < HAND_SIZE; i++)
  {
    printf("Computer Hand: %d, %d\n", computer->hand[i]->suit, computer->hand[i]->value); // read computer hand
  }
  printf("Final Player score: %d\n", evaluate_hand(player->hand)); // get the scores of each hand
  printf("Final Computer score: %d\n", evaluate_hand(computer->hand));

  Player* winner = compare_hands(player, computer); // decide the winner!
  if (winner == player){
    printf("player wins\n");
  } else if (winner == computer) {
    printf("computer wins\n");
  } else {
    printf("its a tie\n");
  }
  
}

int main() {
  test_evaluations();
  return 0;
}
