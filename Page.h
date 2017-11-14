#include "Object.h"

#ifndef PAGE_H
#define PAGE_H

struct element{
        Object *obj;
        element *next;
};
typedef element *List;


class Page: public Object{
        public:
                //function
                Page();
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(unsigned char e, unsigned char d);
                List search(Object &obj);
                void add(Object *obj);
                void remove(Object &obj);
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

void Page::graphics(U8GLIB u8g){
        List p;
        p = F;
        while(p!=NULL){
                (*p).obj->graphics(u8g);
                p = (*p).next;
        }
}

void Page::update(TIME time){
        List p;
        p = F;
        while(p!=NULL){
                (*p).obj->update(time);
                p = (*p).next;
        }
}
void Page::onAction(unsigned char e, unsigned char d){
        List p;
        p = F;
        while(p!=NULL){
                (*p).obj->onAction(e,d);
                p = (*p).next;
        }
}

List Page::search(Object &obj){
        List p;
        p = F;
        while((p!=NULL) && (*p).obj != &obj){
                beforeT = p;
                p = (*p).next;
        }
        return p;
}
void Page::add(Object *obj){
        List p;
        p = new element;
        (*p).obj = obj;
        (*p).next = F;
        F = p;  
        numOfObjs++;
}
void Page::remove(Object &obj){
        List t;
        t = search(obj);
        if(t!=NULL){
                if(F==t) F = (*t).next;
                else (*beforeT).next = (*t).next;
                delete t;
                numOfObjs--;
        }
}
#endif

