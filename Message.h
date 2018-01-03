#include "PageManager.h"

#ifndef MESSAGE_H
#define MESSAGE_H



class Message: public Object{
        public:
                Message(int x, int y, const char *msg);
                ~Message(){
                  delete msg;
                 }
                void showInt(int _score);
                void itoa(int value, char *s);
                //override
                void exec(uint8 id){}
                void graphics(U8GLIB u8g);
                void update(TIME time){}
                void onAction(uint8 button, uint8 down){}
        private:
                const char *msg;
                int score;
                int x, y;
                char str[10];
};
Message::Message(int x, int y, const char *msg):x(x),y(y),msg(msg){
  score = 0;
}

void Message::showInt(int _score){
  score = _score;
}

void Message::itoa(int n, char *s){
  char *p = s,temp;
  for(;n!=0;n/=10){
    *p= n%10 + '0';
    p++;
  }
  *p = '\0';
  p--;
  for(; p>s; s++, p--){
    temp = *p;
    *p = *s;
    *s = temp;
  }
}

void Message::graphics(U8GLIB u8g){
  // show message
  u8g.setFont(u8g_font_04b_03r);
  u8g.drawStr90(x ,y + 32-u8g.getStrWidth(msg)/2, msg);
  // convert score to string
  itoa(score,str);
  // show score
  u8g.setFont(u8g_font_timB08);
  u8g.drawStr90(x-10, y + 32-u8g.getStrWidth(str)/2,str);
}
#endif
