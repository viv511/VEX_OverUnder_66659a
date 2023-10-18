#include "main.h"

#ifndef PID_H
#define PID_H

using namespace pros;

class PID {
    public:
        float target;

        float error;
        bool isTurn;
        float previousError;
        float integral;
        float derivative;
        float output;

        float kP;
        float kI;
        float kD;

        float smallTimeCounter;
        float smallErr;
        float smallExit;
        float largeTimeCounter;
        float largeErr;
        float largeExit;

        float maxCounter;
        float maxTime;

        int cross;

    public:
        PID();
        PID(bool isTurnPID, float sErr, float sExit, float lErr, float lExit, float maxAllowedTime);
        PID(float kP, float kI, float kD, bool isTurnPID, float sErr, float sExit, float lErr, float lExit, float maxAllowedTime);
        PID(float kP, float kD, bool isTurnPID, float sErr, float sExit, float lErr, float lExit, float maxAllowedTime);
        void resetPlease();
        void resetConstants();
        void setConstants(float p, float i, float d);
        float getTarget();
        void setTarget(float t);
        void turnLookupPID(float t);
        void moveLookupPID(float t);
        void setType(bool turn);
        void setExitConditions(float sErr, float sExit, float lErr, float lExit, float maxOut);
        float calculateOutput(float current);
        void updateTimers();
        bool isSettled();
        int numbersign(float num);
        float autoTuneAngle();
};


#endif