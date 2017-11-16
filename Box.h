#include "PageManager.h"
#include "bit.h"

#ifndef BOX_H
#define BOX_H
#define R 11
#define C 11
#define WIDTH 10
#define LVMAX 5

struct MAP{
  uint8 walls[16];
  uint32 boxes;
  uint32 marks;
	uint8 x,y;
};

const static MAP maps[] = {
	{0xff, 0xb, 0x50, 0x90, 0xd2, 0x16, 0x90, 0xaf,
	  0x10, 0x84, 0x20, 0xfc, 0x1, 0x0, 0x0, 0x0,
    0x23,0x21221112,6,6},
  {0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x14,0x15,3,1}
};
const uint8 boxes[] PROGMEM = {
0xff,0xc0,0x80,0x40,0x80,0x40,0x80,0x40,0x80,0x40,
0x80,0x40,0x80,0x40,0x80,0x40,0x80,0x40,0xff,0xc0
};

const uint8 marks[] PROGMEM = {
0, 0, 0, 0, 12, 0, 12, 0, 127, 128, 63, 0, 30, 0, 30, 0, 18, 0, 0, 0
};

struct PERSON{
  uint8 x,y;
};

class Box: public Object{
        public:
                Box();
                void initial(uint8 level);
                void handle(uint8 dx, uint8 dy);
                uint8 step(uint8 dx, uint8 dy);
                void draw(U8GLIB u8g, uint32 A, const uint8 *bitmap);
                //override
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(uint8 e, uint8 d);
        private:
                uint8 x0,y0;
                uint8 level;
                PERSON person;
                MAP map;

};
Box::Box(){
  level = 0;
  initial(level);
}

void Box::initial(uint8 level){
  if(level >= LVMAX) {
    // do sth here
    return;
  }
  map = maps[level];
  person.x = map.x;
  person.y = map.y;
}
void Box::handle(uint8 dx, uint8 dy){
  switch (step(dx,dy)) {
    case 0://!hit
      person.x+=dx;
      person.y+=dy;
      break;
    case 1://hit wall
      break;
    case 2://hit box
      if(step(dx*2,dy*2))
        break;
      else{
        person.x+=dx;
        person.y+=dy;
        mov16(map.boxes, person.x, person.y, dx, dy);
        if(map.boxes == map.marks)
          initial(++level);
        break;
      }
    default: break;
  }
}

uint8 Box::step(uint8 dx, uint8 dy){
  if(get(map.walls, person.x+dx, person.y+dy, C))
    return 1;
  if(get16(map.boxes, person.x+dx, person.y+dy))
    return 2;
  return 0;
}

void Box::draw(U8GLIB u8g, uint32 A, const uint8 *bitmap){
  uint8 i,j;
  while(A != 0){
    i = A%16;
    A/=16;
    j = A%16;
    A/=16;
    u8g.drawBitmapP(i*WIDTH-x0, j*WIDTH-y0, 2, 10, bitmap);
  }
}

void Box::graphics(U8GLIB u8g){
  //draw map
  x0 = person.x*WIDTH - 30;
  y0 = person.y*WIDTH - 30;
  for(int y = 0; y < R; y++)
    for(int x = 0; x < C; x++)
      if(get(map.walls,x,y,C))
        u8g.drawBox(x*WIDTH - x0, y*WIDTH - y0, WIDTH, WIDTH);
  //draw boxes, marks
    draw(u8g, map.boxes, boxes);
    draw(u8g, map.marks, marks);
  //draw person
  u8g.drawFrame(32, 32, WIDTH-4, WIDTH-4);
}
void Box::update(TIME time){
  unsigned long period = time.NOW - time.PREC;
}

void Box::onAction(uint8 e, uint8 d){
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
                        }else{
                                //Back release
                        }
                        break;
                default: break;
        }
}
#endif
