//#include <Arduino.h>
#include <U8glib.h>
#include "PageManager.h"
//#include "Test.h"
#include "Bricks.h"

//U8G2_ST7920_128X64_F_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 13 /* A4 */, /*cs=*/ U8X8_PIN_NONE, /*dc/rs=*/ 12 /* A3 */, /*reset=*/ 15 /* A1 */);    // Remember to set R/W to 0 

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
  //Test *t = new Test(10,10);
  Bricks *t = new Bricks(0,0,64,128);
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

