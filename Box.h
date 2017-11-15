#include "PageManager.h"
#include "bit.h"

#ifndef BOX_H
#define BOX_H
#define R 8
#define C 8
#define WIDTH 10

struct MAP{
  uint64 walls;
  uint64 boxes;
  uint64 marks;
	uint8 x,y;
};
struct PERSON{
  uint8 x,y;
};
MAP maps[] = {
	{0x80ff,0x2400,0x0000,3,1},
	{0,0,0,2,2},
	{0,0,0,3,4}
};

class Box: public Object{
        public:
                Box(int x0, int y0);
                void initial(int level);
                void handle(int dx, int dy);
                int step(int dx, int dy);
                //override
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(unsigned char e, unsigned char d);
        private:
                int x0,y0;
                int level;
                PERSON person;
                MAP map;

};
Box::Box(int x0, int y0):x0(x0),y0(y0){
  level = 0;
  initial(level);
}

void Box::initial(int level){
  map = maps[level];
  person.x = map.x;
  person.y = map.y;
}
void Box::handle(int dx, int dy){
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
        mov(map.boxes, person.x, person.y, dx, dy, C);
        if(map.boxes == map.marks)
          initial(++level);
      }
      break;
    default: break;
  }
}

int Box::step(int dx, int dy){
  if(get(map.walls, person.x+dx, person.y+dy, C) 
      || person.x+dx < 0 || person.x+dx > 7 
      || person.y+dy < 0 || person.y+dy > 7)
    return 1;    
  if(get(map.boxes, person.x+dx, person.y+dy, C))
    return 2;
  return 0;
}

void Box::graphics(U8GLIB u8g){
  //draw map
  for(int y = 0; y < R; y++){
    for(int x = 0; x < C; x++){
      if(get(map.walls,x,y,C))
        u8g.drawBox(x0+x*WIDTH, y0+y*WIDTH, WIDTH, WIDTH);
      if(get(map.boxes,x,y,C))
        u8g.drawFrame(x0+x*WIDTH, y0+y*WIDTH, WIDTH, WIDTH);
      if(get(map.marks,x,y,C))
        u8g.drawBox(x0+x*WIDTH+2, y0+y*WIDTH+2, WIDTH-4, WIDTH-4);
    }
  }
  //draw person
  u8g.drawFrame(x0+person.x*WIDTH+2, y0+person.y*WIDTH+2, WIDTH-4, WIDTH-4);
}
void Box::update(TIME time){
  unsigned long period = time.NOW - time.PREC;
}

void Box::onAction(unsigned char e, unsigned char d){
        switch(e){
                case 1:
                        if(d){
                                //Left press;
                                handle(-1,0);
                                if(x0 + person.x*WIDTH < 2*WIDTH)
                                  x0+=WIDTH;
                        }else{
                                //Left release
                        }
                        break;
                case 2:
                        if(d){
                                handle(1,0);
                                if(x0 + person.x*WIDTH > 5*WIDTH)
                                  x0-=WIDTH;
                                //Right press
                        }else{
                                //Right release
                        }
                        break;
                case 4:
                        if(d){
                                handle(0,-1);
                                y0-=WIDTH;
                                //Top press
                        }else{
                                //Top release
                        }
                        break;
                case 8:
                        if(d){
                                //Bottom press
                                handle(0,1);
                                y0+=WIDTH;
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
