#include "PageManager.h"
#include "Quadrapassel.h"
#include "Sokoban.h"

#ifndef MENUPAGE_H
#define MENUPAGE_H
#define N  2

struct MENU{
    const char *name;
};

MENU menu[N]{
    {"Quadras"},
    {"Sokoban"}
};

 const uint8 sokoban[] PROGMEM = {
     0, 0, 0, 49, 0, 0, 55, 193, 240, 63, 239, 248, 63, 255, 252,
     31, 247, 254, 9, 195, 254, 9, 192, 254, 31, 230, 254, 31, 230, 252,
     63, 225, 252, 63, 193, 248, 55, 0, 112, 48, 0, 96, 0, 0, 32,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31,
     255, 248, 63, 255, 252, 127, 255, 252, 126, 0, 124, 127, 0, 252,
     119, 129, 252, 115, 195, 220, 113, 231, 156, 112, 255, 12, 112, 126,
     28, 112, 60, 12, 120, 126, 12, 112, 255, 28, 113, 231, 156, 127, 195,
     220, 119, 129, 252, 126, 0, 252, 126, 0, 124, 127,
     255, 252, 63, 255, 252, 31, 255, 248, 0, 0, 0
 };
 const uint8 quadra[] PROGMEM = {
   0, 0, 0, 0, 0, 0, 126, 0, 15, 192, 0, 255, 255, 223, 224, 1, 128, 192,
   96, 16, 1, 190, 223, 111, 208, 1, 190, 223, 111, 208, 1, 190, 223, 111,
   208, 1, 190, 223, 111, 208, 1, 190, 223, 111, 208, 1, 128, 192, 96, 48, 1,
   255, 255, 255, 240, 0, 255, 63, 159, 224, 0, 0, 0, 31, 224, 0, 0, 0, 112,
   48, 0, 0, 0, 119, 176, 0, 0, 0, 119, 176, 0, 0, 0, 127, 176, 0, 15, 248,
   119, 176, 0, 8, 12, 114, 48, 0, 9, 236, 63, 240, 0, 13, 236, 31, 224, 0, 13,
   236, 0, 0, 0, 13, 236, 0, 0, 0, 13, 236, 0, 0, 0, 12, 12, 0, 0, 0, 15, 248,
   0, 0, 31, 255, 255, 254, 0, 48, 8, 12, 6, 0, 55, 233, 228, 2, 0, 55, 235, 236,
   250, 0, 55, 235, 236, 250, 0, 55, 239, 236, 250, 0, 55, 233, 236, 250, 0, 49, 12,
   12, 2, 0, 63, 252, 127, 254, 0, 31, 255, 255, 252, 0, 7, 255, 255, 248,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
 };

class MenuPage: public Object{
        public:
                MenuPage(int x, int y);
                void drawIcon(U8GLIB u8g, uint8 i);
                //override
                void exec(uint8 id){}
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(uint8 button, uint8 down);
        private:
                int totalTime;
                bool delay;
                int x,y,ys;
                int choice;
                const uint8 *men;
};

MenuPage::MenuPage(int x, int y):x(x),y(y){
    choice = 0;
    ys = 65;
    delay = false;
}

void MenuPage::drawIcon(U8GLIB u8g, uint8 i){
  switch(i){
    case 0:
        u8g.drawBitmapP(90, 10, 5, 40, quadra);
      break;
    case 1:
        u8g.drawBitmapP(95, 10, 3, 41, sokoban);
      break;
    default: break;
  }
}
void MenuPage::graphics(U8GLIB u8g){
  //draw list option game
  for(int i = 0; i < N; i++){
    if(choice == i){
        u8g.setFont(u8g_font_04b_03r);
        u8g.drawStr90(x-i*15+2, 2, "->");
            u8g.setFont(u8g_font_timB08);
        drawIcon(u8g, choice);
    }
    else
        u8g.setFont(u8g_font_timR08);
    u8g.drawStr90(x-i*15, y + 32-u8g.getStrWidth(menu[i].name)/2,menu[i].name);
  }
  //show information
  u8g.drawLine(55,0, 55, 64);
  u8g.setFont(u8g_font_timB08);
  u8g.drawStr90(40 ,ys,    "GVHD: Huynh  Huu  Hung");
  u8g.drawStr90(28 ,ys, "SV  : Duong  Minh  Nhi - 14T1");
  u8g.drawStr90(16 ,ys, "SV  : Tran  Dinh  Phu - 14T1");
  u8g.drawStr90(4 ,ys, "SV  : Nguyen  Tung  Duc - 14T1");

}

void MenuPage::update(TIME time){
    uint8 period = time.NOW - time.PREC;
    totalTime += period;
    // time to delay
    if(totalTime%2000 < period){
        totalTime = 0;
        delay = !delay;
    }
    else if(totalTime%50 < period && !delay){
        ys-=1;
    }
    if(ys < -100) ys = 65;
}

void MenuPage::onAction(uint8 button, uint8 down){
          switch(button){
                case BUTTON_UP:
                        if(down){
                                choice= (choice == 0)? N-1: choice-1;
                        }else{
                        }
                        break;
                case BUTTON_DOWN:
                        if(down){
                            choice= (choice == N-1)? 0: choice+1;
                        }else{
                        }
                        break;
                case BUTTON_OK:
                        if(down){
                                Object *t;
                                switch(choice){
                                  case 0:
                                    t = new Quadrapassel();
                                    break;
                                  case 1:
                                    t = new Sokoban();
                                    break;
                                  default: break;
                                }
                                Page *p = new Page();
                                p->add(t,GAME_FUNC_ALL);
                                pm->push(p);
                                p->exec(0);
                        }else{
                        }
                        break;
                default: break;
        }
}
#endif
