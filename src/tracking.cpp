#include "tracking.h"

using namespace pros;

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

Waypoint robotPose = Waypoint(0, 0, 0);

void initializeTracking() {
    inertial.reset();
    while(inertial.is_calibrating()){
        delay(10);
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
        deltaTheta = (inertial.get_rotation() - lastTheta) * degToRad;

        robotPose.x += (sin(deltaTheta) * deltaDist);
        robotPose.y += (cos(deltaTheta) * deltaDist);
        robotPose.theta = inertial.get_rotation();

        lastTheta = inertial.get_rotation();

        leftAbsolute += leftChange;
        rightAbsolute += rightChange;

        leftLast = leftCurrent;
        rightLast = rightCurrent;

		lcd::print(1, "X: %f\n", robotPose.x);
        lcd::print(2, "Y: %f\n", robotPose.y);
        lcd::print(3, "Inertial: %f\n", robotPose.theta);

        delay(10);
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

float distDiff(float startX, float startY, float endX, float endY) {
    return (std::sqrt(std::pow(endX - startX, 2) + std::pow(endY - startY, 2))) / 10;
}

float angDiff(float startX, float startY, float endX, float endY) {
    float angleDiff = atan2(endY - startY, endX - startX);

    angleDiff *= radToDeg;
    if (angleDiff < 0) {
        angleDiff += 360;
    }

    angleDiff -= 90;
    angleDiff *= -1;

    return angleDiff;
}