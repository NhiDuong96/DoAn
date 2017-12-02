#include "PageManager.h"

#ifndef MESSAGE_H
#define MESSAGE_H



class Message: public Object{
        public:
                Message(int x, int y, const char *msg);
                void showInt(int _score);
                void itoa(unsigned int value, char *s);
                //override
                void exec(int id){}
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(uint8 e, uint8 d);
        private:
                const char *msg;
                int score;
                bool isshowInt;
                int x, y;
};
Message::Message(int x, int y, const char *msg):x(x),y(y),msg(msg){
  score = 0;
  isshowInt = false;
}
void Message::showInt(int _score){
  score = _score;
  isshowInt = true;
}
void Message::itoa(unsigned int n, char *s){
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
  u8g.setFont(u8g_font_04b_03r);
  u8g.drawStr90(x ,y + 32-u8g.getStrWidth(msg)/2, msg);
  if(isshowInt){
    char str[20];
    itoa(score,str);
    u8g.drawStr90(x-10, y + 32-u8g.getStrWidth(str)/2,str);
  }

}

void Message::update(TIME time){}

void Message::onAction(uint8 e, uint8 d){
          switch(e){
                case 1:
                        if(d){
                                //Left down
                                PageManager::getInstance()->pop();
                                PageManager::getInstance()->peak()->exec(0);
                        }else{
                                //Left up
                        }
                        break;
                case 2:
                        if(d){
                                //Right down
                                PageManager::getInstance()->pop();
                                PageManager::getInstance()->peak()->exec(1);
                        }else{
                                //Right up
                        }
                        break;
                case 4:
                        if(d){
                                //Top down
                        }else{
                                //Top up
                        }
                        break;
                case 8:
                        if(d){
                                //Bottom down
                        }else{
                                //Bottom up
                        }
                        break;
                case 16:
                        if(d){
                                //Ok down
                        }else{
                                //Ok up
                        }
                        break;
                case 32:
                        if(d){
                                //Back down
                        }else{
                                //Back up
                        }
                        break;
                default: break;
        }
}
#endif
