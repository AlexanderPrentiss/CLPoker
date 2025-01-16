#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "evaluate.h"
#include "preprocess.h"

typedef struct player // a player has pocket cards and their final hand to be scored
{
  Card* pocket[POCKET_SIZE];
  Card* hand[HAND_SIZE];
  int balance;
} Player;

// deal_cards fills a players pocket with two cards
void deal_cards(Player* player, Deck* deck) {
  for (int i = 0; i < POCKET_SIZE; i++)
  {
    player->pocket[i] = deck->card_list[deck->index]; // fills each pocket slot with a card
    deck->index++; // make sure to increment the index after each card is dealt
  }
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
      if (check_highest(playerA->hand, HAND_SIZE) > check_highest(playerB->hand, HAND_SIZE)) return playerA; // if there is we just need the highest card
      else if (check_highest(playerA->hand, HAND_SIZE) < check_highest(playerB->hand, HAND_SIZE)) return playerB;
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

int place_bet(Player* player, int amount) {
  while (amount > player->balance) 
  {
    printf("Insufficient funds.\n");
    printf("Try again: ");
    scanf("%d", &amount);
  }
  player->balance = player->balance - amount;
  return amount;
  
}

int decide_bet(Player* computer, int last_bet, int eval) {
  int Threshold = 1 + rand() % (last_bet+1);
  printf("Threshold: %d\n", Threshold);
  int roll = (last_bet + 1) * eval / last_bet;
  printf("Roll: %d\n", roll);
  if (roll > Threshold + (last_bet/4) && computer->balance > last_bet + (last_bet/4)) return place_bet(computer, last_bet+(last_bet/2));
  else if (roll >= Threshold) return place_bet(computer, last_bet);
  else return 0;
}

// Test : This will probably become the play hand function
void play_hand(Deck* deck, Player* player, Player* computer) {
  shuffle_deck(deck); // shuffle the deck

  deal_cards(player, deck); // deal cards to the player and computer
  deal_cards(computer, deck);

  for (int i = 0; i < POCKET_SIZE; i++) {
    printf("Player Pocket: %d, %d\n", player->pocket[i]->suit, player->pocket[i]->value);
  }

  for (int i = 0; i < POCKET_SIZE; i++) {
    printf("Computer Pocket: %d, %d\n", computer->pocket[i]->suit, computer->pocket[i]->value);
  }
 
  // FIRST ROUND OF BETTING
  int playable_size = POCKET_SIZE;
  int pool = 0;
  int player_bet;
  int computer_bet;
  int eval;

  printf("Would you like to bet? amount: ");
  scanf("%d", &player_bet);
  player_bet = place_bet(player, player_bet);
  pool += player_bet;
  printf("Player has bet: %d\n", player_bet);
  
  //Make computer's hand

  eval = check_highest(computer->pocket, playable_size);
  printf("Computer Eval: %d\n", eval);
  computer_bet = decide_bet(computer, player_bet, eval);
  pool += computer_bet;

  if (computer_bet < player_bet) {
    printf("Computer folds, Player wins!");
    player->balance+=pool;
    return;
  }
  printf("Computer bets: %d\n",computer_bet);
  if (computer_bet > player_bet) {
    char* response = (char*) malloc(sizeof(char)*3);
    printf("Do yout want to match %d (yes or no)? ", computer_bet);
    scanf("%s", response);
    if (!strcmp(response,"yes")) {
      pool+=computer_bet-player_bet;
      place_bet(player, computer_bet-player_bet);
    } else {
      computer->balance+=pool;
      printf("Player folds, Computer wins!");
      return;
    }
    free(response);
  }

  //DEALING THE FLOP
  Card* table[HAND_SIZE]; // deal cards for the table
  Card* flop = deal_flop(deck);
  playable_size = HAND_SIZE;
  for (int i = 0; i < FLOP_SIZE; i++)
  {
    table[i] = &flop[i];
    printf("Table (Flop): %d, %d\n", table[i]->suit, table[i]->value);
  }

  //SECOND ROUND OF BETTING
  printf("Would you like to bet? amount: ");
  scanf("%d", &player_bet);
  player_bet = place_bet(player, player_bet);
  pool += player_bet;
  printf("Player has bet: %d\n", player_bet);

  Card* computer_hand[HAND_SIZE+1];
  for (int i = 0; i < HAND_SIZE; i++)
  {
    if (i < POCKET_SIZE)
    {
      computer_hand[i] = computer->pocket[i];
    } else {
      computer_hand[i] = table[i-POCKET_SIZE];//table[i - POCKET_SIZE - 1];
    }
  }

  eval = evaluate_hand(computer_hand);
  printf("Computer Eval: %d\n", eval);
  computer_bet = decide_bet(computer, player_bet, eval);
  pool += computer_bet;

  if (computer_bet < player_bet) {
    printf("Computer folds, Player wins!");
    player->balance+=pool;
    return;
  }
  printf("Computer bets: %d\n",computer_bet);
  if (computer_bet > player_bet) {
    char* response = (char*) malloc(sizeof(char)*3);
    printf("Do yout want to match %d (yes or no)? ", computer_bet);
    scanf("%s", response);
    if (!strcmp(response,"yes")) {
      pool+=computer_bet-player_bet;
      place_bet(player, computer_bet-player_bet);
    } else {
      computer->balance+=pool;
      printf("Player folds, Computer wins!");
      return;
    }
    free(response);
  }

  //DEALING THE TURN
  table[3] = deal_turn(deck);

  for (int i = 0; i < FLOP_SIZE+TURN_SIZE; i++)
  {
    printf("Table (Turn): %d, %d\n", table[i]->suit, table[i]->value);
  }

  //THIRD ROUND OF BETTING
  printf("Would you like to bet? amount: ");
  scanf("%d", &player_bet);
  player_bet = place_bet(player, player_bet);
  pool += player_bet;
  printf("Player has bet: %d\n", player_bet);

  computer_hand[4] = table[3];
  eval = evaluate_hand(computer_hand);
  printf("Computer Eval: %d\n", eval);
  computer_bet = decide_bet(computer, player_bet, eval);
  pool += computer_bet;

  if (computer_bet < player_bet) {
    printf("Computer folds, Player wins!");
    player->balance+=pool;
    return;
  }
  printf("Computer bets: %d\n",computer_bet);
  if (computer_bet > player_bet) {
    char* response = (char*) malloc(sizeof(char)*3);
    printf("Do yout want to match %d (yes or no)? ", computer_bet);
    scanf("%s", response);
    if (!strcmp(response,"yes")) {
      pool+=computer_bet-player_bet;
      place_bet(player, computer_bet-player_bet);
    } else {
      computer->balance+=pool;
      printf("Player folds, Computer wins!");
      return;
    }
    free(response);
  }

  //DEALING THE RIVER
  table[4] = deal_river(deck);
  
  for (int i = 0; i < HAND_SIZE; i++)
  {
    printf("Table (River): %d, %d\n", table[i]->suit, table[i]->value); // read values from the table
  }
  
  set_hand(player, table); // find the best hand for the player and the computer
  set_hand(computer, table);


  //FINAL ROUND OF BETTING
  printf("Would you like to bet? amount: ");
  scanf("%d", &player_bet);
  player_bet = place_bet(player, player_bet);
  pool += player_bet;
  printf("Player has bet: %d\n", player_bet);

  eval = evaluate_hand(computer->hand);
  printf("Computer Eval: %d\n", eval);
  computer_bet = decide_bet(computer, player_bet, eval);
  pool += computer_bet;

  if (computer_bet < player_bet) {
    printf("Computer folds, Player wins!");
    player->balance+=pool;
    return;
  }
  printf("Computer bets: %d\n",computer_bet);
  if (computer_bet > player_bet) {
    char* response = (char*) malloc(sizeof(char)*3);
    printf("Do yout want to match %d (yes or no)? ", computer_bet);
    scanf("%s", response);
    if (!strcmp(response,"yes")) {
      pool+=computer_bet-player_bet;
      place_bet(player, computer_bet-player_bet);
    } else {
      computer->balance+=pool;
      printf("Player folds, Computer wins!");
      return;
    }
    free(response);
  }

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
    player->balance+=pool;
  } else if (winner == computer) {
    printf("computer wins\n");
    computer->balance+=pool;
  } else {
    printf("its a tie\n");
    player->balance+=pool/2;
    computer->balance+=pool/2;
  }
}

int main() {
  Deck* deck = (Deck*) malloc(sizeof(Deck)); //create deck 
  Player* player = (Player*) malloc(sizeof(Player)); // create player
  Player* computer = (Player*) malloc(sizeof(Player)); // create computer

  player->balance = 100;
  computer->balance = 100;

  initialize_deck(deck); // initialize deck
  
  char* play = (char*) malloc(sizeof(char)*3);
  while (!strcmp(play, "yes")) {
    play_hand(deck, player, computer);
    printf("Player balance: %d\nComputer balance: %d\n", player->balance, computer->balance);
    printf("Another hand (yes or no)?: ");
    scanf("%s", play);
  }

  destroy_deck(deck); 
  free(player);
  free(computer);
  return 0;
}
