#include "tracking.h"
#include <iostream>

using namespace pros;

const int TIME_INTERVAL = 10;
constexpr float PI = 3.14159265358979323846;
constexpr float degToRad = PI/180;
constexpr float radToDeg = 180/PI;
constexpr float DIAMETER = 3.32; //3.32
constexpr float TICKS = 36000.0;
constexpr float RATIO = (PI * DIAMETER * 2)/(TICKS * 3); //Account for 2:3 gear ratio

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

float lastAngle = 0;
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
        currentAngle = inertial.get_rotation();
        leftCurrent = leftRot.get_position(); // rotation sensors are flipped
        rightCurrent = rightRot.get_position() * -1;

        leftChange = (leftCurrent - leftLast) * RATIO;
        rightChange = (rightCurrent - rightLast) * RATIO;

        /*
        Approximating X and Y position using two parallel wheels:
        
        Infinite small segement is a linear path.
        Because it is linear, L and R dist is the same. To better approx. this, let's average dL and dR at dT.

        By calculating the angular offset and applying sin and cos to theta, we find the offset in x and y.
        */

        // deltaDist = (leftChange + rightChange) / 2;
        // deltaTheta = (currentAngle - lastAngle);

        // float distTravelled = distance(robotPose, lastRobotPose);

        // robotPose.x += (sin(currentAngle) * deltaDist);
        // robotPose.y += (cos(currentAngle) * deltaDist);
        // robotPose.setTheta(currentAngle * radToDeg);
        // robotPose.setVel(distTravelled / TIME_INTERVAL); //Derivative of Odometry = Velocity

        // float acceleration = (robotPose.getVel() - prevVel) / TIME_INTERVAL;
        // prevVel = robotPose.getVel();

        // lastRobotPose.setX(robotPose.getX());
        // lastRobotPose.setY(robotPose.getY());

        // lastAngle = currentAngle;

        leftAbsolute += leftChange;
        rightAbsolute += rightChange;

        leftLast = leftCurrent;
        rightLast = rightCurrent;

		lcd::print(1, "X: %f\n", leftAbsolute);
        lcd::print(2, "Y: %f\n", rightAbsolute);
        lcd::print(3, "Inertial: %f\n", currentAngle);

        delay(TIME_INTERVAL);
    }
}


float getLeft() {
    return leftAbsolute;
}


float getRight() {
    return rightAbsolute;
}

void setStart(float sX, float sY) {
    robotPose.setX(sX);
    robotPose.setY(sY);
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


