#include "autofuncs.h"
#include "tracking.h"
#include <cmath>

using namespace pros;

float thetaPID = 100;
PID movePID = PID(400, 0, 800, false, 1, 300, 3, 500, 4000);
PID swingPID = PID(true, 1, 300, 3, 300, 1500);
PID turnPID = PID(true, 1, 100, 3, 300, 3000);

void driveDist(float l, float r, float limit, float ang) {
    std::pair<float, float> v = getDist();

    if(l == r) {
        float cur = avgDist();
        float tAng = inertial.get_rotation();

        //Drive forward/backward
        movePID.setTarget(l + cur);
        while(!movePID.isSettled()) {
            float kT = (tAng - inertial.get_rotation()) * thetaPID;
            cur = avgDist();
            float o = movePID.calculateOutput(cur);

            if(movePID.error < 4) {
                kT = 0;
            }
            LeftDT.move_voltage((o - kT)*limit);
            RightDT.move_voltage((o + kT)*limit);
            pros::delay(10);
        }
        LeftDT.move_voltage(0);
        RightDT.move_voltage(0);
        LeftDT.brake();
        RightDT.brake();

    }
    else {
        float o = 0;
        if(l == 0) {
            swingPID.setTarget(ang + inertial.get_rotation());
            while(!swingPID.isSettled()) {
                o = swingPID.calculateOutput(inertial.get_rotation());
                RightDT.move_voltage(-(o*limit));
            
                pros::delay(10);
            }
            LeftDT.move_voltage(0);
            RightDT.move_voltage(0);
            LeftDT.brake();
            RightDT.brake();
        }
        else {
            swingPID.setTarget(ang + inertial.get_rotation());
            while(!swingPID.isSettled()) {
                o = swingPID.calculateOutput(inertial.get_rotation());
                LeftDT.move_voltage(o*limit);
                
                pros::delay(10);
            }
            LeftDT.move_voltage(0);
            RightDT.move_voltage(0);
            LeftDT.brake();
            RightDT.brake();
        }
    }
    
    controller.rumble(".");
}

void turn(float ang) {
    turnPID.setTarget(ang + inertial.get_rotation());
    while(!turnPID.isSettled()) {
        float turnSpeed = turnPID.calculateOutput(inertial.get_rotation());

        LeftDT.move_voltage(turnSpeed);
        RightDT.move_voltage(-turnSpeed);

        pros::delay(10);
    }
}