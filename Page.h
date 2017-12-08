#include "Object.h"

#ifndef PAGE_H
#define PAGE_H

struct element{
        Object *obj;
        unsigned char key;
        element *next;
};
typedef element *List;


class Page: public Object{
        public:
                //function
                Page();
                ~Page(){
                  delete F,beforeT;  
                }
                void exec(int id);
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(unsigned char e, unsigned char d);
                void add(Object *obj, unsigned char key);
                void clear();
                //
        private:
                List F;
                List beforeT;
                int numOfObjs;
};

Page::Page(){
        F = NULL;
        numOfObjs = 0;
}
void Page::exec(int id){
        List p;
        p = F;
        while(p!=NULL){
                (*p).obj->exec(id);
                p = (*p).next;
        }  
}
void Page::graphics(U8GLIB u8g){
        List p;
        p = F;
        while(p!=NULL){
                if((*p).key & 1) (*p).obj->graphics(u8g);
                p = (*p).next;
        }
}

void Page::update(TIME time){
        List p;
        p = F;
        while(p!=NULL){
                if((*p).key  & 2) (*p).obj->update(time);
                p = (*p).next;
        }
}
void Page::onAction(unsigned char e, unsigned char d){
        List p;
        p = F;
        while(p!=NULL){
                if((*p).key & 4) (*p).obj->onAction(e,d);
                p = (*p).next;
        }
}


void Page::add(Object *obj, unsigned char key){
        List p;
        p = new element;
        (*p).obj = obj;
        (*p).key = key;
        (*p).next = F;
        F = p;  
        numOfObjs++;
}
void Page::clear(){
        List p;
        p = F;
        while(p!=NULL){
                delete (*p).obj;
                p = (*p).next;
        }
        numOfObjs = 0;
}
#endif

