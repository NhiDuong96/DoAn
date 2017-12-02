#include "PageManager.h"
#include "bit.h"
#include "Message.h"

#ifndef QUADRAPASSEL_H
#define QUADRAPASSEL_H
#define HEIGHT 18
#define WIDTH 12
#define W 5

struct MATRIX {
  uint8 data[2];
};

MATRIX Shapes[5]= {
  {0x96,0x00},
  {0x92,0x00},
  {0xb2,0x00},
  {0xb4,0x00},
  {0x1b,0x00}
  };

class Quadrapassel: public Object{
        public:
                Quadrapassel();
                //override
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(unsigned char e, unsigned char d);

                void exec(int id);
                //define
                void rot();
                void randShape();
                int insertMap(const uint8 *X, int x, int y);
                int checkMap();
                int hit(const uint8 *X, int x, int y);
        private:
                uint16 map[HEIGHT+1];
                MATRIX shape;
                int xs,ys;
                int x0,y0;
                uint16 totalTime;
                int score;
};
Quadrapassel::Quadrapassel(){
    x0 = 4;
    y0 = 2;
    exec(0);
}

void Quadrapassel::exec(int id){
        switch(id){
          case 0:
            totalTime = 0;
            score = 0;
            map[0]=0xffff;
            for(int i = 1; i <= HEIGHT; i++)
              map[i]=0xe001;
            randShape();        
            break;
          case 1:
            PageManager::getInstance()->pop();
            break;
          default: break;
          
        }
}

void Quadrapassel::graphics(U8GLIB u8g){
//
        u8g.drawFrame(x0,y0,HEIGHT*W,60);
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if((xs+i <= HEIGHT) && get(shape.data,j,i,3))
                    u8g.drawBox(x0+(xs+i-1)*W, y0+(ys+j-1)*W,W,W);
        for(int j = 1; j <= HEIGHT; j++){
                for(int i = 1; i <= WIDTH; i++)
                        if(map[j] >> i & 1)
                                u8g.drawBox(x0+(j-1)*W,y0+(i-1)*W,W,W);
        }
}

void Quadrapassel::update(TIME time){
        uint8 period = time.NOW - time.PREC;
        totalTime += period;
        if(totalTime%1000 < period){
                if(hit(shape.data,xs-1,ys) == 0)  {
                        xs--;
                }else{
                        if(insertMap(shape.data,xs,ys)) {
                                score+=checkMap();
                                randShape();
                        }else{
                            //
                            Message *msg1 = new Message(50,0,"Your Score:");
                            msg1->showInt(score);
                            Message *msg2 = new Message(30,0,"<Replay | Exit>");
                            Page *p = new Page();
                            p->add(msg1);
                            p->add(msg2);
                            PageManager::getInstance()->push(p);
                        }
                }
        }
}
void Quadrapassel::onAction(unsigned char e, unsigned char d){
        switch(e){
                case 1:
                        if(d){
                                //Left down
                                if(hit(shape.data,xs,ys-1) == 0)
                                        ys--;
                        }else{
                                //Left up
                        }
                        break;
                case 2:
                        if(d){
                                //Right down
                                if(hit(shape.data,xs,ys+1) == 0)
                                        ys++;
                        }else{
                                //Right up
                        }
                        break;
                case 4:
                        if(d){
                                //Top down
                                rot();
                                ys = (ys + 3 > WIDTH)?(WIDTH-2):ys;
                        }else{
                                //Top up
                        }
                        break;
                case 8:
                        if(d){
                                //Bottom down
                                while(hit(shape.data,xs-1,ys) == 0){
                                        xs--;
                                }
                                if(insertMap(shape.data,xs,ys)) {
                                        score+=checkMap();
                                        randShape();
                                }else{
                                  Message *msg1 = new Message(50,0,"Your Score:");
                                  msg1->showInt(score);
                                  Message *msg2 = new Message(30,0,"<Replay | Exit>");
                                  Page *p = new Page();
                                  p->add(msg1);
                                  p->add(msg2);
                                  PageManager::getInstance()->push(p);
                                }
                        }else{
                                //Bottom up
                        }
                        break;
                case 16:
                        if(d){
                                //Ok down
                        }else{
                                //Ok up
                        }
                        break;
                case 32:
                        if(d){
                                //Back down
                                PageManager::getInstance()->pop();
                        }else{
                                //Back up
                        }
                        break;
                default: break;
        }
}

void Quadrapassel::rot(){
    MATRIX c = {0,0};
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(get(shape.data,j,i,3))
                set(c.data,i,2-j,3);
    shape = c;
}

void Quadrapassel::randShape(){
        shape = Shapes[random(0,5)];
        xs = HEIGHT+1;
        ys = random(1,WIDTH-2);
}

int Quadrapassel::insertMap(const uint8 *X, int x, int y){
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++){
            if(x+i > HEIGHT) return 0;
            if(get(shape.data,j,i,3))
                map[x+i] |= 1 << (y+j);
        }
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
                        while(is < HEIGHT){//note
                                map[is] = map[is+1];
                                is++;
                        }
                        map[is] = 0;
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

