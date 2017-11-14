#include "PageManager.h"

static const uint8_t cross_block_bits[] U8X8_PROGMEM  = {
  0xFF, 0x3F, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 
  0xC1, 0x20, 0xC1, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 
  0x01, 0x20, 0xFF, 0x3F, };

class Test: public Object{
        public:
                Test(float x, float y);
                void graphics(U8G2 u8g2);
                void update(TIME time);
                void onAction(unsigned char e, unsigned char d);
        private:
                float x,y;
                int vx, vy;
};
Test::Test(float x, float y):x(x),y(y){
        vx = vy = 0;
}

void Test::graphics(U8G2 u8g2){
        u8g2.drawXBMP(x, y, 14, 14, cross_block_bits);
}

void Test::update(TIME time){
        unsigned long period = time.NOW - time.PREC;
        
        x += vx*float(period)/50;
        y += vy*float(period)/50;
}
void Test::onAction(unsigned char e, unsigned char d){
        switch(e){
                case 1:
                        if(d){
                                //Left press
        x+=10;
                        }else{
                                //Left release
        x-=10;
                        }
                        break;
                case 2:
                        if(d){
                                //Right down
        x-=10;
                        }else{
                                //Right up
        x+=10;
                        }
                        break;
                case 4:
                        if(d){
                                //Top down
        y+=10;
                        }else{
                                //Top up
        y-=10;
                        }
                        break;
                case 8:
                        if(d){
                                //Bottom down
       y-=10;
                        }else{
                                //Bottom up
       y+=10;
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

