#include <U8glib.h>
#include "variables.h"

#ifndef OBJECT_H
#define OBJECT_H
// game timer
struct TIME{
        uint16 START;
        uint16 PREC;
        uint16 NOW;
};
class Object{
        public:
        virtual ~Object(){};
        //game life
        virtual void exec(uint8 id) = 0;
        virtual void graphics(U8GLIB u8g) = 0;
        virtual void update(TIME time) = 0;
        virtual void onAction(uint8 button, uint8 down) = 0;
};
#endif

