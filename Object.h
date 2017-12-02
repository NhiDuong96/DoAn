#include <U8glib.h>

#ifndef OBJECT_H
#define OBJECT_H

struct TIME{
        unsigned long START;
        unsigned long PREC;
        unsigned long NOW;
};

class Object{
        public:
        //game life
        virtual void exec(int id) = 0;
        virtual void graphics(U8GLIB u8g) = 0;
        virtual void update(TIME time) = 0;
        virtual void onAction(unsigned char e, unsigned char d) = 0;
};
#endif

