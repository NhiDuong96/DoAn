#include "MenuPage.h"

U8GLIB_ST7920_128X64_4X u8g(10,9,8,7,6,5,4,3,11,13,12);
PageManager *pm = PageManager::getInstance();
struct TIME time;
int btn[6] = {A1,A5,A4,A3,A2,A0};
int num = 6;


void setup(void) {
  u8g.setRot180();
  time.START = time.NOW = millis();
  for(int i=0 ;i <num; i++)
    pinMode(btn[i], INPUT);
    MenuPage *t = new MenuPage(75,0);
    Page *p = new Page();
    p->add(t,7);
    pm->push(p);
}
char prec_state = 0;

void loop(void) {
  // picture loop
 u8g.firstPage();
  do {
    char state = 0;
    for(int i = 0; i < num ; i++)
      state |= digitalRead(btn[i]) << i;
    char e = state ^ prec_state;
    if(e){
        pm->peak()->onAction(e,e&state);
        prec_state = state;
    }
    time.PREC = time.NOW;
    time.NOW = millis();
    pm->peak()->update(time);
    pm->peak()->graphics(u8g);

  } while ( u8g.nextPage() );
  delay(1);
}

