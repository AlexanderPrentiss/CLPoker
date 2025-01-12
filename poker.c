#include<stdio.h>
#include<stdlib.h>

#define DECK_SIZE 52
#define POCKET_SIZE 2
#define HAND_SIZE 5
#define FLOP_SIZE 3

typedef enum {Clubs, Diamonds, Spades, Hearts} Suit;

typedef struct card
{
  Suit suit;
  int value;
} Card;

int main() {
  return 0;
}
