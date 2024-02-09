#include "main.h"

#ifndef GLOBALS
#define GLOBALS
using namespace pros;

extern Motor FL;
extern Motor FR;
extern Motor ML;
extern Motor MR;
extern Motor BL;
extern Motor BR;

extern Motor cata;

extern Motor intake;

extern Controller controller;

extern IMU inertial;
extern Rotation leftRot;
extern Rotation rightRot;

extern ADIDigitalOut wings;
extern ADIDigitalOut backWing;
extern ADIDigitalOut intakePiston;

extern Motor_Group LeftDT;
extern Motor_Group RightDT;

#endif