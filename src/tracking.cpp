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
        
        pros::lcd::print(0, "Inertial: %f\n", inertial.get_rotation());
		pros::lcd::print(1, "Left: %f\n", leftRot.get_position());
        pros::lcd::print(2, "Right: %f\n", rightRot.get_position());

        pros::delay(10);
    }
}