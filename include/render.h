#ifndef render_h
#define render_h

#define CARD_WIDTH 7
#define CARD_HEIGHT 5

void clr();

void clr_txt();

void set_crsr_home(int posx, int posy);

void rtrn_home(); 

void draw_card(int posx, int posy, int suit, int value);

#endif
