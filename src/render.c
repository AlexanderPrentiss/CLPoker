#include<stdio.h>
#include "render.h"

#define CARD_WIDTH 9
#define CARD_HEIGHT 7

void clr() {
  printf("\e[1J");
}

void draw_card(int posx, int posy, int suit, int value) {
  printf("\e[s");
  if (suit == 10) {
    printf("\e[%d;%df", posy, posx+1);
    for (int i = posx; i < posx + CARD_WIDTH-2; i++) printf("_");

    printf("\e[%d;%df/", posy+1, posx);
    for (int i = 2; i < CARD_WIDTH;i++) printf("#");
    printf("\\");
   
    for (int i = posy + 2; i < posy + CARD_HEIGHT; i++)
    {
      printf("\e[%d;%df|", i, posx);
      for (int j = 1; j < CARD_WIDTH-1;j++) printf("#");
      printf("|");
  }

  printf("\e[%d;%df\\", posy + CARD_HEIGHT, posx);
  for (int i = 2; i < CARD_WIDTH;i++) printf("#");
  printf("\e[u\n");
  return;
  }
  char suit_L;
  switch(suit){
    case 0:
      suit_L = 'D';
      break;
    case 1:
      suit_L = 'H';
      break;
    case 2:
      suit_L = 'C';
      break;
    case 3:
      suit_L = 'S';
      break;
  }
  int digits = 1;
  if (value/10 > 0) digits = 2;
  printf("\e[%d;%df", posy, posx+1);
  for (int i = posx; i < posx + CARD_WIDTH-2; i++) printf("_");

  printf("\e[%d;%df/%d", posy+1, posx, value);
  for (int i = 2; i < CARD_WIDTH-(2*digits);i++) printf(" ");
  printf("%d\\", value);
 
  int off = 0;
  for (int i = posy + 2; i < posy + CARD_HEIGHT; i++)
  {
    printf("\e[%d;%df|", i, posx);
    for (int j = 1; j < CARD_WIDTH/2;j++) printf(" ");
    if (i == posy + (CARD_HEIGHT + 1)/2) {
      printf("%c", suit_L);
      off = 1;
    }
    for (int j = posx + off; j < posx + CARD_WIDTH/2;j++) printf(" ");
    off = 0;
    printf("|");
  }

  printf("\e[%d;%df\\%d", posy + CARD_HEIGHT, posx, value);
  for (int i = 2; i < CARD_WIDTH-(2*digits);i++) printf("_");
  printf("%d/", value);
  printf("\e[u\n");
}

