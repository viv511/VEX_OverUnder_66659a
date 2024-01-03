#include "tracking.h"
#include <iostream>

using namespace pros;

const int TIME_INTERVAL = 10;
constexpr float PI = 3.14159265358979323846;
constexpr float degToRad = PI/180;
constexpr float radToDeg = 180/PI;
constexpr float DIAMETER = 3.32;
constexpr float TICKS = 36000.0;
constexpr float RATIO = (PI * DIAMETER)/TICKS;

float leftAbsolute = 0;
float leftCurrent = 0;
float leftChange = 0;
float leftLast = 0;

float rightAbsolute = 0;
float rightCurrent = 0;
float rightChange = 0;
float rightLast = 0;

float deltaDist = 0;
float deltaTheta = 0;

float lastTheta = 0;
float currentAngle = 0;

Waypoint robotPose = Waypoint(0, 0, 0);
Waypoint lastRobotPose = Waypoint(0, 0, 0);

float prevVel = 0;

void initializeTracking() {
    inertial.reset();
    int timer = 0;
    while(inertial.is_calibrating()){
        delay(10);
        timer += 10;

        if(timer > 4000) {
            controller.rumble("-----------------");
            break;
        }
    }
    inertial.set_rotation(0);

    leftRot.reset_position();
    rightRot.reset_position();
    leftRot.set_position(0);
    rightRot.set_position(0);
}

void tracking() {
    initializeTracking();

    while(true) {
        currentAngle = inertial.get_rotation() * degToRad;
        leftCurrent = leftRot.get_position();
        rightCurrent = rightRot.get_position();

        leftChange = (leftCurrent - leftLast) * RATIO;
        rightChange = (rightCurrent - rightLast) * RATIO;

        /*
        Approximating X and Y position using two parallel wheels:
        
        Infinite small segement is a linear path.
        Because it is linear, L and R dist is the same. To better approx. this, let's average dL and dR at dT.

        By calculating the angular offset and applying sin and cos to theta, we find the offset in x and y.
        */

        deltaDist = (leftChange + rightChange)/2;
        deltaTheta = (currentAngle - lastTheta);

        float distTravelled = distance(robotPose, lastRobotPose);

        robotPose.x += (sin(currentAngle) * deltaDist);
        robotPose.y += (cos(currentAngle) * deltaDist);
        robotPose.setTheta(currentAngle);
        robotPose.setVel(distTravelled / TIME_INTERVAL); //Derivative of Odometry = Velocity

        float acceleration = (robotPose.getVel() - prevVel) / TIME_INTERVAL;
        prevVel = robotPose.getVel();

        lastRobotPose.setX(robotPose.getX());
        lastRobotPose.setY(robotPose.getY());

        lastTheta = currentAngle;

        leftAbsolute += leftChange;
        rightAbsolute += rightChange;

        leftLast = leftCurrent;
        rightLast = rightCurrent;

		lcd::print(1, "X: %f\n", robotPose.x);
        lcd::print(2, "Y: %f\n", robotPose.y);
        lcd::print(3, "Inertial: %f\n", robotPose.theta * radToDeg);

        delay(TIME_INTERVAL);

    }
}

float avgDist() {
    return ((leftAbsolute + rightAbsolute) / 2.0);
}

void set() {
    leftAbsolute = 0;
    rightAbsolute = 0;
}

Waypoint getCurrentPose() {
    return (Waypoint)robotPose;
}


