#include "main.h"

#ifndef AUTOFUNCS_H
#define AUTOFUNCS_H

using namespace pros;

void driveDist(float d);
void driveDist(float d, float limit);
void driveTime(int time);
void turn(float ang);
void stopMotors();
void arcade(double a, double b);
void pivot(double angle);

#endif