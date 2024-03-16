#ifndef gridBox_h
#define gridBox_h

#include <Arduino.h>

class spot{
    public:
        int cost;
        bool l;
        bool r;
        bool u;
        bool d;
        spot();
};



#endif
