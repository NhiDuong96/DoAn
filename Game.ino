#include "MenuPage.h"
#define NUM_BTN 6

//device connection
U8GLIB_ST7920_128X64_4X u8g(10,9,8,7,6,5,4,3,11,13,12);
int btn[NUM_BTN] = {A1,A5,A4,A3,A2,A0};

struct TIME time;
char prec_state = 0;


void setup(void) {
  u8g.setRot180();
  // set pin input
  for(int i=0 ;i < NUM_BTN; i++)
    pinMode(btn[i], INPUT);
  // game timer
  time.START = time.NOW = millis();
  // add game menu to display
  MenuPage *t = new MenuPage(75,0);
  Page *p = new Page();
  p->add(t,GAME_FUNC_ALL);
  pm->push(p);
}

void loop(void) {
  // picture loop
 u8g.firstPage();
  do {
    char state = 0;
    //read input of button
    for(int i = 0; i < NUM_BTN ; i++)
      state |= digitalRead(btn[i]) << i;
    //check if there is a event happened
    char e = state ^ prec_state;
    if(e){
    // handle game event
        pm->peak()->onAction(e,e&state);
        prec_state = state;
    }
    time.PREC = time.NOW;
    time.NOW = millis();
    // update game state
    pm->peak()->update(time);
    // draw game object
    pm->peak()->graphics(u8g);

  } while ( u8g.nextPage() );
  delay(1);
}

