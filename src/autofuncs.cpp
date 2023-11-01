#include "autofuncs.h"
#include "tracking.h"
#include <cmath>
#include <vector>
#include <utility>

using namespace pros;

float thetaPID = 160;
double degToRad = 3.14159265358979323846/180; //M_PI sobad
float expectedAngle = 0;
PID movePID = PID(false, 1, 300, 3, 500, 4000);
PID swingPID = PID(true, 1, 300, 3, 300, 1500);
PID turnPID = PID(true, 1, 100, 3, 300, 1200);

void driveDist(float l, float r, float limit, float ang) {
    pair<float, float> v = getDist();

    if(l == r) {
        float cur = avgDist();
        float tAng = inertial.get_rotation();

        //Drive forward/backward
        movePID.setTarget(l + cur);
        while(!movePID.isSettled()) {
            float kT = (tAng - inertial.get_rotation()) * thetaPID;
            cur = avgDist();
            float o = movePID.calculateOutput(cur);

            // if(movePID.error < 5) {
            //     kT = 0;
            // }
            LeftDT.move_voltage((o + kT)*limit);
            RightDT.move_voltage((o - kT)*limit);
            pros::delay(10);
        }
        stopMotors();
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
            stopMotors();
        }
        else {
            swingPID.setTarget(ang + inertial.get_rotation());
            while(!swingPID.isSettled()) {
                o = swingPID.calculateOutput(inertial.get_rotation());
                LeftDT.move_voltage(o*limit);
                
                pros::delay(10);
            }
            stopMotors();
        }
    }
    
    controller.rumble(".");
}

void turn(float ang) {
    // set inertial to 0 at the start of each route instead of each turn  
    
    expectedAngle += ang;
    float turnAngle = expectedAngle - inertial.get_rotation();
    while (turnAngle <= -180) { // ensures it lies between -180 and 180
        turnAngle += 360;
    }
    while (turnAngle > 180) {
        turnAngle -= 360;
    }

    turnPID.setTarget(turnAngle);
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

vector<pair<float, float>> generatePath(float carrotDist, float startX, float startY, float startAng, float endX, float endY, float endAng){
	float actualDist = sqrt(pow((endX - startX), 2) + pow((endY - startY), 2));

	//Calculate carrot point
	float carrotX = endX - actualDist * sin(endAng * degToRad) * carrotDist;  
	float carrotY = endY - actualDist * cos(endAng * degToRad) * carrotDist;

	vector<pair<float, float>> follow;
	for(float t=0.00; t<=1.00; t+=0.01) {
		float oneMinus = 1 - t;
		float pathX = oneMinus * (oneMinus * startX + t * carrotX) + t * (oneMinus * carrotX + t * endX);
		float pathY = oneMinus * (oneMinus * startY + t * carrotY) + t * (oneMinus * carrotY + t * endY);
		follow.push_back(make_pair(pathX, pathY));
	}

    return follow;
}