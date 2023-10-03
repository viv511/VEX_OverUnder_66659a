#include "main.h"

#ifndef GLOBALS
#define GLOBALS
using namespace pros;

extern pros::Motor FL;
extern pros::Motor FR;
extern pros::Motor ML;
extern pros::Motor MR;
extern pros::Motor BL;
extern pros::Motor BR;

extern pros::Motor Cata1;
extern pros::Motor Cata2;

extern pros::Controller controller;

extern pros::IMU inertial;
extern pros::Rotation leftRot;
extern pros::Rotation rightRot;

extern pros::ADIDigitalOut wings;
extern pros::ADIDigitalOut intake;

extern pros::Motor_Group LeftDT;
extern pros::Motor_Group RightDT;
extern pros::Motor_Group cata;

#endif