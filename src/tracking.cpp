#include "tracking.h"

using namespace pros;

constexpr float PI = 3.141592;
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
float rX = 0;
float rY = 0;

float lastTheta = 0;

void initializeTracking() {
    inertial.reset();
    while(inertial.is_calibrating()){
        pros::delay(10);
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
        leftCurrent = -1 * leftRot.get_position();
        rightCurrent = -1 * rightRot.get_position();

        leftChange = (leftCurrent - leftLast) * RATIO;
        rightChange = (rightCurrent - rightLast) * RATIO;

        /*
        Approximating X and Y position using two parallel wheels:
        
        Infinite small segement is a linear path.
        Because it is linear, L and R dist is the same. To better approx. this, let's average dL and dR at dT.

        By calculating the angular offset and applying sin and cos to theta, we find the offset in x and y.
        */

        deltaDist = (leftChange + rightChange)/2;
        deltaTheta = inertial.get_rotation() - lastTheta;

        rX += (sin(deltaTheta) * deltaDist);
        rY += (cos(deltaTheta) * deltaDist);

        lastTheta = inertial.get_rotation();

        leftAbsolute += leftChange;
        rightAbsolute += rightChange;

        leftLast = leftCurrent;
        rightLast = rightCurrent;

        pros::lcd::print(0, "Inertial: %f\n", inertial.get_rotation());
		pros::lcd::print(1, "X: %f\n", rX);
        pros::lcd::print(2, "Y: %f\n", rY);

        pros::delay(10);
    }
}

std::pair<float, float> getDist() {
    return std::make_pair(leftAbsolute, rightAbsolute);
}

float avgDist() {
    return ((leftAbsolute + rightAbsolute) / 2.0);
}