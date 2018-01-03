#include "bit.h"
#include "data.h"

#ifndef SOKOBAN_H
#define SOKOBAN_H
#define WIDTH 12
#define LVMAX 5

#define SIZE 35
#define WALLS 0
#define BOXES 15
#define MARKS 23
#define X 31
#define Y 32
#define R 33
#define C 34

const uint8 MAP[][SIZE] PROGMEM= {
  {
    28, 20, 244, 135, 225, 47, 40, 56,0,0,0,0,0,0,0,
    0x35,0x54,0x43,0x33,0,0,0,0,
    0x64,0x36,0x41,0x13,0,0,0,0,
    4,4,8,8
},
 {
    222, 20, 205, 99, 24, 254,0,0,0,0,0,0,0,0,0,
    0x23,0x32,0x43,0x52,0x63,0,0,0,
    0x51,0x61,0x62,0x63,0x64,0,0,0,
    2,2,8,6
},
  {
    30, 114, 66, 215, 149, 161, 129, 255,0,0,0,0,0,0,0,
    0x23,0x52,0x65,0,0,0,0,0,
    0x41,0x51,0x61,0,0,0,0,0,
    2,1,8,8
    },
  {
    240, 62, 69, 10, 180, 91, 164, 64, 159, 224,0,0,0,0,0,
    0x32,0x52,0x53,0,0,0,0,0,
    0x46,0x56,0x66,0,0,0,0,0,
    4,3,9,9
    },
  {
    255, 19, 226, 33, 56, 130, 98, 45, 194, 42, 40, 128, 50, 232, 255,
    0x37,0x39,0x47,0x59,0x68,0x69,0x75,0x76,
    0x11,0x12,0x13,0x14,0x21,0x22,0x23,0x24,
    9,4,10,12
  }
  };

const uint8 wall[] PROGMEM = {
  249, 240, 255, 240, 255, 240, 255, 240, 255, 240, 121,
  224, 121, 224, 255, 240, 255, 240, 255, 240, 255, 240, 249, 240
};

const uint8 box[] PROGMEM = {
  63, 192, 127, 224, 224, 112, 208, 176, 201, 48, 198, 48, 198,
  48, 201, 48, 208, 176, 224, 112, 127, 224, 63, 192
};

const uint8 mark[] PROGMEM = {
  0, 0, 0, 0, 0, 0, 6, 0, 6, 0, 25, 128, 25, 128, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0
};

const uint8 menr[] PROGMEM = {
  0, 0, 0, 0, 228, 0, 230, 0, 63, 112, 63, 80, 31, 112, 243, 0, 225, 0, 129, 0, 0, 0, 0, 0
};
const uint8 menl[] PROGMEM = {
  0, 0, 0, 0, 129, 0, 225, 0, 243, 0, 31, 112, 63, 80, 63, 112, 230, 0, 228, 0, 0, 0, 0, 0
};
const uint8 mens[] PROGMEM = {
  0, 0, 0, 0, 142, 0, 227, 0, 63, 112, 31, 80, 63, 112, 227, 0, 142, 0, 0, 0, 0, 0, 0, 0
};


class Sokoban: public Object{
        public:
                Sokoban();
                ~Sokoban(){
                  delete men;
                }
                void initial(int level);
                void handle(uint8 dx, uint8 dy);
                uint8 step(uint8 dx, uint8 dy);
                void drawMap(U8GLIB u8g, uint8 *A, const uint8 *bitmap);
                int fixed();
                //override
                void exec(uint8 id);
                void graphics(U8GLIB u8g);
                void update(TIME time){}
                void onAction(uint8 button, uint8 down);
        private:
                uint8 x0,y0;
                int level;
                uint8 map[SIZE];
                const uint8 *men;

};
Sokoban::Sokoban(){
  level = 0;
}

void Sokoban::exec(uint8 id){
  switch(id){
    case 0:
      initial(level);
      break;
    case 1:// next level
      initial(++level);
      break;
    case 2:
      initial(level);
      break;
    case 3:// back to previous page
      pm->pop();
      break;
    case 11:
      initial(level);
      break;
    case 12:// back to previous page
      pm->pop();
      break;
    case 13://next level
      initial(++level);
      break;
    default: break;
  }
}

void Sokoban::initial(int level){
  if(level >= LVMAX) {
    // you passed all level
    Message *master = new Message(75,0,"Congratulation!");
    Page *p = new Page();
    p->add(master,GAME_JUST_DRAW);
    pm->push(p);
    return;
  }
  // person state
  men = mens;
  // load data from flash memory
  for(int i = 0; i < SIZE; i++){
    // =))
    map[i] = pgm_read_byte(&(*MAP)[i]+level*SIZE);
  }
}

void Sokoban::handle(uint8 dx, uint8 dy){
  switch (step(dx,dy)) {
    case 0://!hit
        //person step
        map[X]+=dx;
        map[Y]+=dy;
        men = mens;
      break;
    case 1://hit wall
      break;
    case 2://hit box
      if(step(dx*2,dy*2))
        break;
      else{
        //person step
        map[X]+=dx;
        map[Y]+=dy;
        //person state
        if(dx == 0) men = (dy == 1)? menr : menl;
        //box step, update map
        mov16(map + BOXES, map[X], map[Y], dx, dy);
        //you passed this map
        if(fixed()){
          //show next level message
          msgNextLevel->showInt(level+1);
          Page *page = new Page();
          page->add(msgNextLevel,GAME_JUST_DRAW);
          page->add(lNextLevel,GAME_NO_UPDATE);
          pm->push(page);
        }
        break;
      }
    default: break;
  }
}

uint8 Sokoban::step(uint8 dx, uint8 dy){
  if(get(map + WALLS, map[X]+dx, map[Y]+dy, map[C]))
    return 1;
  if(get16(map + BOXES, map[X]+dx, map[Y]+dy))
    return 2;
  return 0;
}

int Sokoban::fixed(){
  uint8 i,j,k=0;
  uint8 *A = map+BOXES;
  while(*(A+k) != 0 && k < 8){
    i = *(A+k)%16;
    j = *(A+k)/16;
    k++;
    if(get16(map+MARKS,i,j) == 0) return 0;
  }
  return 1;
}

void Sokoban::drawMap(U8GLIB u8g, uint8 *A, const uint8 *bitmap){
  uint8 i,j;
  uint8 k = 0;
  while(*(A+k) != 0 && k < 8){
    i = *(A+k)%16;
    j = *(A+k)/16;
    k++;
    u8g.drawBitmapP(i*WIDTH-x0, j*WIDTH-y0, 2, WIDTH, bitmap);
  }
}

void Sokoban::graphics(U8GLIB u8g){
  //draw map
  x0 = map[X]*WIDTH - 64;
  y0 = map[Y]*WIDTH - 32;
  for(int y = 0; y < map[R]; y++)
    for(int x = 0; x < map[C]; x++)
      if(get(map + WALLS,x,y,map[C]))
        u8g.drawBitmapP(x*WIDTH-x0, y*WIDTH-y0, 2, WIDTH, wall);
  //draw boxes, marks
    drawMap(u8g, map + BOXES, box);
    drawMap(u8g, map + MARKS, mark);
  //draw person
  u8g.drawBitmapP(64, 32, 2, 12, men);
}

void Sokoban::onAction(uint8 button, uint8 down){
        switch(button){
                case BUTTON_LEFT:
                        if(down){
                                handle(0,-1);
                        }else{
                        }
                        break;
                case BUTTON_RIGHT:
                        if(down){
                                handle(0,1);
                        }else{
                        }
                        break;
                case BUTTON_UP:
                        if(down){
                                handle(1,0);
                        }else{
                        }
                        break;
                case BUTTON_DOWN:
                        if(down){
                                handle(-1,0);
                        }else{
                        }
                        break;
                case BUTTON_OK:
                        break;
                case BUTTON_BACK:
                        if(down){
                            Page *page = new Page();
                            page->add(lOption,GAME_NO_UPDATE);
                            pm->push(page);
                        }else{
                        }
                        break;
                default: break;
        }
}
#endif
