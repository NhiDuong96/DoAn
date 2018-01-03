#include "bit.h"
#include "data.h"

#ifndef QUADRAPASSEL_H
#define QUADRAPASSEL_H
#define HEIGHT 18
#define WIDTH 12
#define W 5

struct MATRIX {
  uint8 data[2];
};

const MATRIX Shapes[5]= {
  {0x96,0x00},
  {0x92,0x00},
  {0xb2,0x00},
  {0xb4,0x00},
  {0x1b,0x00}
  };

const uint8 cube[] PROGMEM = {
    0xf8,0x88,0xA8,0x88,0xf8
};

class Quadrapassel: public Object{
        public:
                Quadrapassel();
                ~Quadrapassel(){
                  }
                //override
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(uint8 e, uint8 d);

                void exec(uint8 id);
                //define
                void rotate();
                void rand();
                void built();
                int insertMap(const uint8 *X, int x, int y);
                int checkMap();
                int hit(const uint8 *X, int x, int y);
        private:
                uint16 map[HEIGHT+1];
                MATRIX shape;
                int xs,ys;
                int x0,y0;
                uint8 v;
                uint16 totalTime;
                int score;
                int hard_level;
};
Quadrapassel::Quadrapassel(){
    x0 = 4;
    y0 = 2;
}

void Quadrapassel::exec(uint8 id){
        switch(id){
          case 0:// initialize map
            totalTime = 0;
            score = 0;
            hard_level = 1000;
            map[0]=0xffff;
            for(int i = 1; i <= HEIGHT; i++)  map[i]=0xe001;
            v = random(0,5);
            rand();
            //add message "score" and "time"
            pm->peak()->add(msgScore,GAME_JUST_DRAW);
            pm->peak()->add(msgTime,GAME_JUST_DRAW);
            break;
          case 1://exit
            pm->pop();
            break;
          case 11:
            exec(0);
            break;
          case 12://exit
            pm->pop();
            break;
          default: break;

        }
}

void Quadrapassel::graphics(U8GLIB u8g){
        u8g.drawFrame(x0,y0,HEIGHT*W,60);
        // show next cube
        MATRIX next = Shapes[v];
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if(get(next.data,j,i,3))
                    u8g.drawBitmapP(110+(i-1)*W, 10+(j-1)*W,1,W,cube);
        // show cube
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if((xs+i <= HEIGHT) && get(shape.data,j,i,3))
                    u8g.drawBitmapP(x0+(xs+i-1)*W, y0+(ys+j-1)*W,1,W,cube);
        // show map
        for(int j = 1; j <= HEIGHT; j++){
            for(int i = 1; i <= WIDTH; i++)
                if(map[j] >> i & 1)
                    u8g.drawBitmapP(x0+(j-1)*W,y0+(i-1)*W,1,W,cube);
        }
}

void Quadrapassel::update(TIME time){
        uint8 period = time.NOW - time.PREC;
        totalTime += period;
        //after 100s increase hard_level
        if(totalTime/1000 > 100) hard_level = 500;
        //drop cube
        if(totalTime%hard_level < period){
                msgTime->showInt(totalTime/hard_level);
                if(hit(shape.data,xs-1,ys) == 0)  xs--;
                else  built();
        }
}
void Quadrapassel::onAction(uint8 button, uint8 down){
        switch(button){
                case BUTTON_LEFT:
                        if(down){
                                if(hit(shape.data,xs,ys-1) == 0)
                                        ys--;
                        }else{
                        }
                        break;
                case BUTTON_RIGHT:
                        if(down){
                                if(hit(shape.data,xs,ys+1) == 0)
                                        ys++;
                        }else{
                        }
                        break;
                case BUTTON_UP:
                        if(down){
                                rotate();
                                if(ys+3 > WIDTH)
                                  ys = WIDTH-2;
                                else if(ys <= 0)
                                  ys = 1;
                        }else{
                        }
                        break;
                case BUTTON_DOWN:
                        if(down){
                            while(hit(shape.data,xs-1,ys) == 0)  xs--;
                            built();
                        }else{
                        }
                        break;
                case BUTTON_OK:
                        break;
                case BUTTON_BACK:
                        if(down){
                            // show option page
                            Page *page = new Page();
                            page->add(lOption,GAME_NO_UPDATE);
                            pm->push(page);
                        }else{
                        }
                        break;
                default: break;
        }
}

void Quadrapassel::rotate(){
    uint8 c[2] = {0,0};
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(get(shape.data,j,i,3))
                set(c,i,2-j,3);
    shape.data[0] = c[0];
    shape.data[1] = c[1];
}

void Quadrapassel::rand(){
        shape = Shapes[v];
        v = random(0,5);
        xs = HEIGHT+1;
        ys = random(1,WIDTH-2);
}

void Quadrapassel::built(){
   if(insertMap(shape.data,xs,ys)) {
       score+=checkMap();
       msgScore->showInt(score);
       rand();
   }else{
        Page *page = new Page();
        msgrltScore->showInt(score);
        page->add(msgrltScore,GAME_JUST_DRAW);
        page->add(lQuadra,GAME_NO_UPDATE);
        pm->push(page);
   }
}

int Quadrapassel::insertMap(const uint8 *X, int x, int y){
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            if(x+i > HEIGHT) return 0;
            if(get(shape.data,j,i,3))
                map[x+i] |= 1 << (y+j);
        }
    return 1;
}

int Quadrapassel::checkMap(){
        int is = -1;
        int i = 1;
        int score = 0;
        while(i <= HEIGHT){
                if(map[i] == 0) break;
                if(map[i] == 0xffff){
                        is = i;
                        score+= WIDTH;
                        while(is < HEIGHT){
                                map[is] = map[is+1];
                                is++;
                        }
                        map[is] = 0xe001;
                }
                else    i++;
        }
        return score;
}

int Quadrapassel::hit(const uint8 *X, int x, int y){
    uint8 Y[2] = {0,0};
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            if(x+i <= HEIGHT && (map[x+i] >> (y+j) & 1))
                set(Y,j,i,3);
        }
    return  (Y[0]&X[0] || Y[1]&X[1]);
}

#endif

