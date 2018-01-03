#include "PageManager.h"
#include "Message.h"
#include "List.h"

#ifndef DATA_H
#define DATA_H

PageManager *pm = PageManager::getInstance();

Message *msgrltScore = new Message(75,0,"Your Score:");
Message *msgScore = new Message(122,10,"Score:");
Message *msgTime = new Message(105,10,"Time:");
Message *msgNextLevel = new Message(75,0,"Passed Level:");


MList *lQuadra = new MList(50 ,0 ,0 ,"Try Again$Exit$");
MList *lNextLevel =new MList(50,0,1,"Next Level$Replay$Exit$");
MList *lOption =new MList(80,0,10,"Back$Reset$Exit$Next$");

#endif
