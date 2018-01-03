#include "PageManager.h"

#ifndef MList_H
#define MList_H

struct elementL{
    char str[15];
    elementL *next;
};
typedef elementL *Queue;

class MList: public Object{
        public:
                MList(int x, int y, uint8 key, const char *msg);
                ~MList(){
                  delete msg;
                  delete Front,Rear;
                }
                //override
                void exec(uint8 id){}
                void graphics(U8GLIB u8g);
                void update(TIME time){};
                void onAction(uint8 button, uint8 down);
        private:
                const char *msg;
                int x, y;
                uint8 size;
                uint8 choice;
                uint8 key;
                Queue Front,Rear;
};
MList::MList(int x, int y, uint8 key, const char *msg):x(x),y(y),key(key),msg(msg){
    size = 0;
    choice = 0;
    Front = NULL;
    Rear = NULL;
    //split msg to list[choice]
    Queue p;
    p =  new elementL;
    char c;
    int i = 0;
    int j = 0;
    do{
        c = *(msg+i);
        if(c == '$'){
            (*p).str[j] = '\0';
            if(Front == NULL) Front = p;
            else (*Rear).next = p;
            Rear = p;
            p =  new elementL;
            j = 0;
            size++;
        }
        else{
            (*p).str[j] = c;
            j++;
        }
        i++;
    }while(c != '\0');
}

void MList::graphics(U8GLIB u8g){
    Queue p;
    p = Front;
    int i = 0;
    while(i < size){
        if(choice == i){
            u8g.setFont(u8g_font_04b_03r);
            u8g.drawStr90(x-i*15+2, 2, "->");
            u8g.setFont(u8g_font_timB08);
        }
        else
            u8g.setFont(u8g_font_timR08);
        u8g.drawStr90(x-i*15, y + 32-u8g.getStrWidth((*p).str)/2,(*p).str);
        p = (*p).next;
        i++;
    }
}

void MList::onAction(uint8 button, uint8 down){
          switch(button){
                case BUTTON_UP:
                        if(down){
                            choice= (choice == 0)? size-1: choice-1;
                        }else{
                        }
                        break;
                case BUTTON_DOWN:
                        if(down){
                            choice= (choice == size-1)? 0: choice+1;
                        }else{
                        }
                        break;
                case BUTTON_OK:
                        if(down){
                                PageManager::getInstance()->pop();
                                PageManager::getInstance()->peak()->exec(key+choice);
                        }else{
                        }
                        break;
                default: break;
        }
}
#endif
