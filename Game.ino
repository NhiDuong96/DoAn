#include <U8glib.h>
#include "PageManager.h"
//#include "Bricks.h"
#include "Box.h"

U8GLIB_ST7920_128X64_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 13, 12, 3);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
PageManager *pm = PageManager::getInstance();
struct TIME time;
int btn[4] = {A2,A3,A4,A5};
int num = 4;
void setup(void) {
  u8g.setRot180();
  time.START = time.NOW = millis();
  for(int i=0 ;i <num; i++) 
    pinMode(btn[i], INPUT);
  Page *page = new Page();
  //Bricks *t = new Bricks(0,0,64,128);
  Box *t = new Box(0,0);
  page->add(t);
  pm->push(page);

  Serial.begin(9600);
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

