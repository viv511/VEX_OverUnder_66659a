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

extern Motor Cata1;
extern Motor Cata2;

extern Controller controller;

extern IMU inertial;
extern Rotation leftRot;
extern Rotation rightRot;

extern ADIDigitalOut wings;
extern ADIDigitalOut intake;

extern Motor_Group LeftDT;
extern Motor_Group RightDT;
extern Motor_Group cata;

#endif