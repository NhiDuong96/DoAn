#include "PageManager.h"
#include "bit.h"

#ifndef SOKOBAN_H
#define SOKOBAN_H
#define WIDTH 12
#define LVMAX 5

struct MAP{
  uint8 walls[30];
  uint64 boxes;
  uint64 marks;
	uint8 x,y;
  uint8 r,c;
};

const static MAP maps[] = {
 
  {
    28, 20, 244, 135, 225, 47, 40, 56,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0, 
    0x35544333,0x64364113,4,4,8,8
  },
	{
	  0xff, 0xb, 0x50, 0x90, 0xd2, 0x16, 0x90, 0xaf,0x10, 0x84, 
	  0x20, 0xfc, 0x1, 0x0, 0x0, 0x0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0x64423423,0x21221112,6,6,11,11
    },
   {
    240, 1, 128, 8, 0, 68, 0, 56, 6, 64, 
    32, 128, 107, 241, 71, 251, 48, 0, 128, 223, 
    45, 132, 224, 63, 252, 1,0,0,0,0, 
    0x00,0x00,12,8,11,19    
    }
};

const uint8 wall[] PROGMEM = {
127, 224, 255, 240, 255, 240, 255, 240, 255, 240, 249, 240, 249, 240, 255, 240, 255, 240, 255, 240, 255, 240, 127, 224
};

const uint8 box[] PROGMEM = {
  63, 192, 127, 224, 224, 112, 208, 176, 201, 48, 198, 48, 198, 48, 201, 48, 208, 176, 224, 112, 127, 224, 63, 192
};

const uint8 mark[] PROGMEM = {
0, 0, 0, 0, 12, 0, 12, 0, 127, 128, 63, 0, 30, 0, 30, 0, 18, 0, 0, 0, 0, 0, 0, 0
};

const uint8 menr[] PROGMEM = {
  14, 0, 10, 0, 14, 0, 0, 0, 15, 192, 31, 0, 62, 0, 14, 0, 15, 128, 61, 128, 49, 128, 49, 192
};
const uint8 menl[] PROGMEM = {
  7, 0, 5, 0, 7, 0, 0, 0, 63, 0, 15, 128, 7, 192, 7, 0, 31, 0, 27, 192, 24, 192, 56, 192
};
const uint8 mens[] PROGMEM = {
  7, 0, 5, 0, 7, 0, 0, 0, 15, 128, 31, 192, 23, 64, 23, 64, 7, 0, 13, 128, 8, 128, 24, 192
};

struct PERSON{
  uint8 x,y;
};

class Sokoban: public Object{
        public:
                Sokoban();
                void initial(uint8 level);
                void handle(uint8 dx, uint8 dy);
                uint8 step(uint8 dx, uint8 dy);
                void draw(U8GLIB u8g, uint64 A, const uint8 *bitmap);
                int fixed();
                //override
                void exec(int id);
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(uint8 e, uint8 d);
        private:
                uint8 x0,y0;
                uint8 level;
                PERSON person;
                MAP map;
                const uint8 *men;

};
Sokoban::Sokoban(){
  level = 0;
  men = mens;
  initial(level);
}

void Sokoban::exec(int id){
  switch(id){
    case 0:
      initial(++level);
      break;
    case 1:
      initial(level);
      break;
    default: break;
  }
}

void Sokoban::initial(uint8 level){
  if(level >= LVMAX) {
    // do sth here
    return;
  }
  men = mens;
  map = maps[level];
  person.x = map.x;
  person.y = map.y;
}
void Sokoban::handle(uint8 dx, uint8 dy){
  switch (step(dx,dy)) {
    case 0://!hit
      person.x+=dx;
      person.y+=dy;
      men = mens;
      break;
    case 1://hit wall
      break;
    case 2://hit Sokoban
      if(step(dx*2,dy*2))
        break;
      else{
        person.x+=dx;
        person.y+=dy;
        mov16(map.boxes, person.x, person.y, dx, dy);
        if(dy == 0)
          men = (dx == 1)? menr : menl;
        if(fixed()){
                            Message *msg1 = new Message(50,0,"Next Level:");
                            msg1->showInt(level);
                            Message *msg2 = new Message(30,0,"<Replay | PlayNext>");
                            Page *p = new Page();
                            p->add(msg1);
                            p->add(msg2);
                            PageManager::getInstance()->push(p);
        }
        break;
      }
    default: break;
  }
}

uint8 Sokoban::step(uint8 dx, uint8 dy){
  if(get(map.walls, person.x+dx, person.y+dy, map.c))
    return 1;
  if(get16(map.boxes, person.x+dx, person.y+dy))
    return 2;
  return 0;
}

int Sokoban::fixed(){
  uint8 i,j;
  uint64 A = map.boxes;
  while(A != 0){
    i = A & 0x0F; A = A >> 4;
    j = A & 0x0F; A = A >> 4;
    if(get16(map.marks,i,j) == 0) return 0;
  }
  return 1;
}

void Sokoban::draw(U8GLIB u8g, uint64 A, const uint8 *bitmap){
  uint8 i,j;
  while(A != 0){
    i = A & 0x0F; A = A >> 4;
    j = A & 0x0F; A = A >> 4;
    u8g.drawBitmapP(i*WIDTH-x0, j*WIDTH-y0, 2, WIDTH, bitmap);
  }
}

void Sokoban::graphics(U8GLIB u8g){
  //draw map
  x0 = person.x*WIDTH - 64;
  y0 = person.y*WIDTH - 32;
  for(int y = 0; y < map.r; y++)
    for(int x = 0; x < map.c; x++)
      if(get(map.walls,x,y,map.c))
        u8g.drawBitmapP(x*WIDTH-x0, y*WIDTH-y0, 2, WIDTH, wall);
  //draw boxes, marks
    draw(u8g, map.boxes, box);
    draw(u8g, map.marks, mark);
  //draw person
  u8g.drawBitmapP(64, 32, 2, 12, men);
}
void Sokoban::update(TIME time){
  unsigned long period = time.NOW - time.PREC;
}

void Sokoban::onAction(uint8 e, uint8 d){
        switch(e){
                case 1:
                        if(d){
                                //Left press;
                                handle(-1,0);
                        }else{
                                //Left release
                        }
                        break;
                case 2:
                        if(d){
                                handle(1,0);
                                //Right press
                        }else{
                                //Right release
                        }
                        break;
                case 4:
                        if(d){
                                handle(0,-1);
                                //Top press
                        }else{
                                //Top release
                        }
                        break;
                case 8:
                        if(d){
                                //Bottom press
                                handle(0,1);
                        }else{
                                //Bottom release
                        }
                        break;
                case 16:
                        if(d){
                                //Ok press
                        }else{
                                //Ok release
                        }
                        break;
                case 32:
                        if(d){
                                //Back press
                                PageManager::getInstance()->pop();
                        }else{
                                //Back release
                        }
                        break;
                default: break;
        }
}
#endif
