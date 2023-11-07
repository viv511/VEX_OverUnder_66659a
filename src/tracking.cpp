#include "tracking.h"

using namespace pros;


constexpr float PI = 3.14159265358979323846;
constexpr float degToRad = PI/180;
constexpr float radToDeg = 180/PI;
constexpr float DIAMETER = 3.32;
constexpr float TICKS = 36000.0;
constexpr float RATIO = (PI * DIAMETER)/TICKS;
constexpr float DEGTORAD = PI/180; 

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
float rX = 0;
float rY = 0;

float lastTheta = 0;

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
        deltaTheta = (inertial.get_rotation() - lastTheta) * DEGTORAD;

        rX += (sin(deltaTheta) * deltaDist);
        rY += (cos(deltaTheta) * deltaDist);

        lastTheta = inertial.get_rotation();

        leftAbsolute += leftChange;
        rightAbsolute += rightChange;

        leftLast = leftCurrent;
        rightLast = rightCurrent;

        lcd::print(0, "Inertial: %f\n", inertial.get_rotation());
		lcd::print(1, "L: %f\n", leftAbsolute);
        lcd::print(2, "R: %f\n", rightAbsolute);

        delay(10);
    }
}

std::pair<float, float> getDist() {
    return std::make_pair(leftAbsolute, rightAbsolute);
}

float avgDist() {
    return ((leftAbsolute + rightAbsolute) / 2.0);
}

void set() {
    leftAbsolute = 0;
    rightAbsolute = 0;
}

std::pair<float, float> getCoords() {
    return std::make_pair(rX, rY);
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