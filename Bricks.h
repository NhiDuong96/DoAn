#include "PageManager.h"

#ifndef BRICKLAYING_H
#define BRICKLAYING_H
#define HEIGHT 25
#define WIDTH 14

struct MATRIX {
  int col;
  int row;
  unsigned char data;
};

MATRIX Shapes[4]= {
  {2,4,0x57},
  {2,3,0x1D},
  {2,3,0x1E},
  {2,2,0x0F}
  };
class BrickLaying: public Object{
        public:
                BrickLaying(int x, int y, int w, int h);
                //override
                void graphics(U8GLIB u8g);
                void update(TIME time);
                void onAction(unsigned char e, unsigned char d);

                void init();
                //define
                void randShape();
                int insertMap(int x, int y,const MATRIX &X);
                int checkMap();
                int movable(const MATRIX &X, int x, int y, int dx, int dy);
        private:
                int x,y,w,h;
                MATRIX shape;
                uint16_t map[HEIGHT];
                int xs,ys;
                unsigned long totalTime;
                int score;
};
BrickLaying::BrickLaying(int x, int y, int w, int h):x(x),y(y),w(w),h(h){
        init();
}

void BrickLaying::init(){
        totalTime = 0;
        score = 0;
        for(int i = 0; i < HEIGHT; i++) map[i]=0;
        randShape();
}
void BrickLaying::graphics(U8GLIB u8g){
//        char buf[5] = {'\0'};
//    my_utoa(buf,totalTime/1000,10);
//    char buf2[5]={'\0'};
//    my_utoa2(buf2,score,10);
//
//    u8g2.setFont(u8g_font_04b_03r);
//    u8g2.drawStr90(105,22,"Time: ");
//    u8g2.drawStr90(115,22,"Score: ");
//
//        u8g2.drawStr90(105,42,buf);
//        u8g2.drawStr90(115,48,buf2);
//
        u8g.drawLine(100,0,100,60);

        for(int i = 0; i < shape.row*shape.col; i++){
                int px = xs+i/shape.col*4;
                if((shape.data >> i & 1) && (px < 100))
                        u8g.drawBox(px,ys+(i%shape.col)*4,4,4);
        }
        for(int j = 0; j < 25; j++){
                for(int i =0 ; i< 16; i++)
                        if(map[j] >> i & 1)
                                u8g.drawBox(j*4,i*4,4,4);
        }
}

void BrickLaying::update(TIME time){
        unsigned int period = time.NOW - time.PREC;
        totalTime += period;
        if(totalTime%1000 < period){//1s
                if(movable(shape,xs/4,ys/4,-1,0) == 0)  {
                        xs-=4;
                }else{
                        if(insertMap(xs/4,ys/4,shape)) {
                                score+=checkMap();
                                randShape();
                        }else{

                        }
                }
        }
}
void BrickLaying::onAction(unsigned char e, unsigned char d){
        switch(e){
                case 1:
                        if(d){
                                //Left down
                                if(movable(shape,xs/4,ys/4,0,-1) == 0)
                                        ys-=4;
                        }else{
                                //Left up
                        }
                        break;
                case 2:
                        if(d){
                                //Right down
                                if(movable(shape,xs/4,ys/4,0,1) == 0)
                                        ys+=4;
                        }else{
                                //Right up
                        }
                        break;
                case 4:
                        if(d){
                                //Top down
//                              rotate90L(shape);
                                ys = (ys/4 + shape.col - 16 > 0)?(16-shape.col)*4:ys;
                        }else{
                                //Top up
                        }
                        break;
                case 8:
                        if(d){
                                //Bottom down
                                while(movable(shape,xs/4,ys/4,-1,0) == 0){
                                        xs-=4;
                                }
                                if(insertMap(xs/4,ys/4,shape)) {
                                        checkMap();
                                        randShape();
                                }else{

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
                        }else{
                                //Back up
                        }
                        break;
                default: break;
        }
}
void BrickLaying::randShape(){
        shape = Shapes[random(0,4)];
        xs = 100;
        ys = random(0,16-shape.col)*4;
}
int BrickLaying::insertMap(int x, int y,const MATRIX &X){
        for(int i = 0; i < X.col*X.row; i++){
                if(x+i/X.col > 24) return 0;
                map[x+i/X.col] |= (X.data >> i & 1)? (1 << (y+i%X.col)) : 0;
        }
        return 1;
}

int BrickLaying::checkMap(){
        int is = -1;
        int i = 0;
        int score = 0;
        while(i < 25){
                if(map[i] == 0) break;
                if(map[i] == 65535)     {
                        is = i;
                        score+=16;
                        while(is < 24){
                                map[is] = map[is+1];
                                is++;
                        }
                        map[is] = 0;
                }
                else    i++;
        }
        return score;
}
int BrickLaying::movable(const MATRIX &X, int x, int y, int dx, int dy){
        x+=dx;
        y+=dy;
        if(x < 0 || y < 0 || y+X.col > 16) return 1;
        unsigned char Y = 0;
        for(int i = 0; i < X.col*X.row; i++)
                if(x+i/X.col < 25)
                        Y |= (map[x+i/X.col] >> (y+i%X.col) & 1)?(1 << i):0;
        return  (Y&X.data);
}

#endif

