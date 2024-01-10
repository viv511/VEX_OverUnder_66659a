#include "main.h"

#ifndef AUTOFUNCS_H
#define AUTOFUNCS_H

using namespace pros;

void swing(bool left, float d, float otherMotor);
void arc(float dist, float ang, float time);
void driveDist(float d);
void driveDist(float d, float limit);
void driveTime(int time);
void turn(float ang);
void stopMotors();
void boomerang(float dX, float dY, float sAng, float eAng, float carrotD);
vector<std::pair<float, float>> generatePath(float carrotDist, float startX, float startY, float startAng, float endX, float endY, float endAng);
std::pair<float, float> calculateWheelVelocity(float sX, float sY, float eX, float eY);
vector<std::pair<float, float>> computeVelocites(vector<std::pair<float, float>> path);

void arcade(double a, double b);

void moveToPoint(Waypoint target, bool isReversed);
void pivot(double angle);

void goToPoint(float tX, float tY);

void swervePoint(bool left, float ICR_x, float ICR_y, float angle, bool direction, float maxVel);

#endif