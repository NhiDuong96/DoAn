#include "Page.h"

#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

struct elementP{
        Page *page;
        elementP *next;
};
typedef elementP *Stack;

class PageManager{
        public:
                PageManager();
                static PageManager* getInstance();
                void push(Page *page);
                void pop();
                Page* peak();
        private:
                static PageManager *m_instance;
                Stack S;
                int numOfPages;
};
PageManager* PageManager::m_instance = NULL;

PageManager* PageManager::getInstance(){
        if(m_instance == NULL) {
                m_instance = new PageManager();
        }
        return m_instance;
}
PageManager::PageManager(){
        S = NULL;
        numOfPages = 0;
}

void PageManager::push(Page *page){
        Stack p;
        p = new elementP;
        (*p).page = page;
        (*p).next = S;
        S = p;
        numOfPages++;
}
void PageManager::pop(){
        Stack p;
        if(S!=NULL){
                p = S;
                S = (*p).next;
                delete p;
                numOfPages--;
        }
}
Page* PageManager::peak(){
        return (S!=NULL) ? (*S).page : NULL;
}
#endif

