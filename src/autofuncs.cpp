#include "autofuncs.h"
#include "tracking.h"
#include <cmath>
#include <vector>
#include <utility>

using namespace pros;

constexpr float WHEEL_BASE = 12.1;
constexpr float PI = 3.14159265358979323846;
constexpr float degToRad = PI/180;
constexpr float radToDeg = 180/PI;

constexpr float botWide = 11.75; 

float thetaPID = 400;
PID movePID = PID(false, 1, 100, 3, 300, 2000);
PID turnPID = PID(true, 1, 100, 3, 300, 1200);

void driveDist(float d) {
    set();
   
    float cur = avgDist();
    float tAng = inertial.get_rotation();
    //Drive forward/backward
    movePID.setTarget(d + cur);
    while(!movePID.isSettled()) {
        float kT = (tAng - inertial.get_rotation()) * thetaPID;
        cur = avgDist();
        float o = movePID.calculateOutput(cur);
        LeftDT.move_voltage((o + kT));
        RightDT.move_voltage((o - kT));
        pros::delay(10);
    }
    stopMotors();
}

void driveDist(float d, float limit) {
    LeftDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    RightDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
   
    float cur = avgDist();
    float tAng = inertial.get_rotation();

    movePID.setTarget(d + cur);
    while(!movePID.isSettled()) {
        float kT = (tAng - inertial.get_rotation()) * thetaPID;
        cur = avgDist();
        float o = movePID.calculateOutput(cur);
        LeftDT.move_voltage((o + kT)*limit);
        RightDT.move_voltage((o - kT)*limit);
        pros::delay(10);
    }
    stopMotors();
}

void driveTime(int time) {
    int dir;
    if(time < 0){
        dir = -1;
        time *= -1;
    } else{
        dir = 1;
    }
    LeftDT.move_voltage(12000*dir);
    RightDT.move_voltage(12000*dir);
    pros::delay(time);
    stopMotors();
}

void turn(float ang) {
    LeftDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    RightDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);

    turnPID.setTarget(ang + inertial.get_rotation());
    while(!turnPID.isSettled()) {
        float turnSpeed = turnPID.calculateOutput(inertial.get_rotation());

        LeftDT.move_voltage(turnSpeed);
        RightDT.move_voltage(-turnSpeed);

        pros::delay(10);
    }
    stopMotors();
}

void stopMotors() {
    LeftDT.move_voltage(0);
    RightDT.move_voltage(0);
    LeftDT.brake();
    RightDT.brake();
}


void pivot(double angle) {
    double theta = 0;
    double startAngle = inertial.get_rotation();

    int quotient;
    if(startAngle < 0){
        quotient = -1 * int(startAngle / 360) + 1;
        startAngle = startAngle + quotient * 360;
    }
    else{
        quotient = int(startAngle / 360);
        startAngle = startAngle - quotient * 360;
    }

    theta = angle - startAngle;
    if(fabs(theta) >= 180){
        if(theta > 0){
            theta = -1 * (360 - theta);
        }
        else{
            theta = 360 + theta;
        }
    }

    turn(theta);
}

void arcade(double a, double b){
    double leftPower = (a+b);
    double rightPower = (a-b);

    LeftDT.move_voltage(leftPower);
    RightDT.move_voltage(rightPower);
}