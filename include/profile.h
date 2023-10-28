#include "main.h"

#ifndef PID_H
#define PID_H

using namespace pros;

class profile {
    public:
        float maxVel;
        float maxAccel;
        float dist;

    public:
        profile(float v, float a);
        void generate(float d);
};



#endif