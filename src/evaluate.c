#include<stdlib.h>
#include "deck.h"
#include "evaluate.h"
#include "preprocess.h"

//check for flush
int check_flush(Card* hand[HAND_SIZE])
{
  int test = hand[0]->suit; // set the suit to be the first card's suit
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

int check_highest(Card** hand, int count){ 
  int highest = hand[0]->value;
  for (int i = 1; i < count; i++) // loop through hand to find the highest card
  {
    if (hand[i]->value > highest) highest = hand[i]->value;
  }
  return highest;
}

// evaluate hand checks to see what score a hand will get
int evaluate_hand(Card* hand[HAND_SIZE]) { // use the data we get from the check functions to score the hand
  if (check_flush(hand) && check_straight(hand) && check_highest(hand, DECK_SIZE) == 14) return royal_flush; // if there is an ace and a straight flush; royal flush!
  else if (check_flush(hand) && check_straight(hand)) return straight_flush; // if there is a straight and a flush its a straight flush
  else if (check_flush(hand)) return flush; // check for straight
  else if (check_straight(hand)) return straight; // check for flush
  else // if its none of those check for pairs/toak/foak/fullhouse
  {
    switch (check_multi(hand)[2])
    {
      case 0:
        return check_highest(hand, HAND_SIZE);
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
  return check_highest(hand, HAND_SIZE); // If its nothing above just return the highest card
}
