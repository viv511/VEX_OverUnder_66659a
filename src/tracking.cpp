#include "tracking.h"

using namespace pros;

void initializeTracking() {
    inertial.reset();
    pros::delay(2000);
    inertial.set_rotation(0);

    leftRot.reset();
    rightRot.reset();
    leftRot.set_position(0);
    rightRot.set_position(0);
}

void tracking() {
    initializeTracking();

    while(true) {


        Task::delay(10);
    }
}