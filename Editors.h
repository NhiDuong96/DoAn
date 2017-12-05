#include "PageManager.h"

#ifndef EDITORS_H
#define EDITORS_H



class Editors: public Object{
        public:
                Editors(int x);
                //override
                void exec(int id){}
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(uint8 e, uint8 d);
        private:
            unsigned long totalTime;
            int x,y;
            bool delay;
};
Editors::Editors(int x):x(x){
    totalTime = 0;
    y = 65;
    delay = false;
}


void Editors::graphics(U8GLIB u8g){
  u8g.drawLine(55,0, 55, 64);
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr90(x ,y,    "GVHD: Huynh Huu Hung");
  u8g.drawStr90(x-12 ,y, "SV  : Duong Minh Nhi - 14T1");
  u8g.drawStr90(x-24 ,y, "SV  : Tran Dinh Phu - 14T1");
  u8g.drawStr90(x-36 ,y, "SV  : Nguyen Tung Duc - 14T1");

}

void Editors::update(TIME time){
    uint8 period = time.NOW - time.PREC;
    totalTime += period;
    if(totalTime%2000 < period){
        totalTime = 0;
        delay = !delay;
    }
    else if(totalTime%100 < period && !delay){
        y-=1;
    }
    if(y < -100) y = 65;
}

void Editors::onAction(uint8 e, uint8 d){}
#endif
