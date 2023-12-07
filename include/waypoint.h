#include "main.h"

#ifndef WAYPOINT_H
#define WAYPOINT_H

using namespace pros;

class Waypoint {
    public:
        float x;
        float y;
        float theta;
        float dist;
        float vel;
        float curv;

    public:
        Waypoint(float xCoord, float yCoord);
        Waypoint(float xCoord, float yCoord, float angleTheta);

        float getX();
        float getY();
        float getTheta();
        float getDist();
        float getVel();
        float getCurv();

        void setX(float newX);
        void setY(float newY);
        void setTheta(float newTheta);
        void setDist(float newDist);
        void setVel(float newVel);
        void setCurv(float newCurv);

        Waypoint scalarMult(Waypoint P, float s);
        float distance(Waypoint A, Waypoint B);
        float angle(Waypoint A, Waypoint B);
        Waypoint normalizeVect(Waypoint P);
        Waypoint getDirVector(Waypoint A, Waypoint B);
        float dotProduct(Waypoint A, Waypoint B);
        float getLength(Waypoint P);
        int numbersign(float num);
        
};


#endif