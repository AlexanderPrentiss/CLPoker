#ifndef preprocess_h
#define preprocess_h

#define DECK_SIZE 52
#define NUM_SUITS 4
#define NUM_VALUES 13
#define POCKET_SIZE 2
#define HAND_SIZE 5
#define FLOP_SIZE 3
#define TURN_SIZE 1
#define RIVER_SIZE 1
#define LOWEST_CARD 2


typedef enum {Clubs, Diamonds, Spades, Hearts} Suit; // mostly for comparing hands
typedef enum {Joker=11, Queen, King, Ace} Value; // Ace high will be default
typedef enum {high, pair = 15, two_pair, three_of_a_kind, straight, flush, full_house, four_of_a_kind, straight_flush, royal_flush} Hand;

#endif
