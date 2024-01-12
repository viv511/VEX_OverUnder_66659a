#include "autofuncs.h"
#include "tracking.h"
#include "waypoint.h"
#include <cmath>
#include <vector>
#include <utility>

using namespace pros;

constexpr float WHEEL_BASE = 12.1; //pls measure dist between Left and right wheel in inches :)
constexpr float PI = 3.14159265358979323846;
constexpr float degToRad = PI/180;
constexpr float radToDeg = 180/PI;

constexpr float botTHICKNESS = 11.75; 


float thetaPID = 160;
PID movePID = PID(false, 1, 100, 3, 300, 2000);
PID swingPID = PID(true, 1, 300, 3, 300, 1500);
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

    controller.rumble(".");
}

void driveDist(float d, float limit) {
    LeftDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    RightDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
   
    float cur = avgDist();
    float tAng = inertial.get_rotation();
    //Drive forward/backward
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

    controller.rumble(".");
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

float expectedAngle = 0;

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

vector<pair<float, float>> generatePath(float carrotDist, float startX, float startY, float startAng, float endX, float endY, float endAng){
	float actualDist = sqrt(pow((endX - startX), 2) + pow((endY - startY), 2));

	//Calculate carrot point
	float carrotX = endX - actualDist * sin(endAng * degToRad) * carrotDist;  
	float carrotY = endY - actualDist * cos(endAng * degToRad) * carrotDist;

	vector<std::pair<float, float>> follow;
	for(float t=0.00; t<=1.00; t+=0.01) {
		float oneMinus = 1 - t;
		float pathX = oneMinus * (oneMinus * startX + t * carrotX) + t * (oneMinus * carrotX + t * endX);
		float pathY = oneMinus * (oneMinus * startY + t * carrotY) + t * (oneMinus * carrotY + t * endY);
		follow.push_back(make_pair(pathX, pathY));
	}

    return follow;
}



std::pair<float, float> calculateWheelVelocity(float sX, float sY, float eX, float eY) {
    // float d = distDiff(sX, sY, eX, eY);
    // float theta = angDiff(sX, sY, eX, eY);

    // //10 = dT = 10msec wait time
    // float v = d / 10;
    // float w = theta / 10;

    // v *= 100000;
    // w *= 1000;
    
    // float leftVel = v - (w * WHEEL_BASE) / 2;
    // float rightVel = v + (w * WHEEL_BASE) / 2;

    return make_pair(1, 1);
}

vector<std::pair<float, float>> computeVelocites(vector<std::pair<float, float>> path) {
    vector<std::pair<float, float>> velocities;

    for(int i=0; i<path.size()-1; i++) {
        float startX = path[i].first;
        float startY = path[i].second;
        float endX = path[i+1].first;
        float endY = path[i+1].second;

        velocities.push_back(calculateWheelVelocity(startX, startY, endX, endY));
    }

    return velocities;
}

void boomerang(float dX, float dY, float sAng, float eAng, float carrotD) {
    //{X,Y} Coordinates
    vector<std::pair<float, float>> path = generatePath(carrotD, 0, 0, sAng, dX, dY, eAng);

    //{X, Y} ==> {L, R} Velocities
    path = computeVelocites(path);


    for(int i=0; i<path.size(); i++) {
        LeftDT.move_voltage(path[i].first);
        RightDT.move_voltage(path[i].second);

        pros::delay(10);
    }

}

void arc(float dist, float ang, float time) {
    float t = 0;
    while(t < ang) {
        LeftDT.move_voltage(10000);
        RightDT.move_voltage(10000);
        pros::delay(10);
        t+=10;
    }
    t = 0;
    while(t < dist-ang) {
        if(time > 0) {
            LeftDT.move_voltage(10000);
            RightDT.move_voltage(2000);
        }
        else {
            LeftDT.move_voltage(2000);
            RightDT.move_voltage(10000);
        }
        pros::delay(10);
        t+=10;
    }


//     set();
//     inertial.set_rotation(0);
//     movePID.setTarget(dist);
//     float oAng = 0;
//     float timer = 0;
//     float kT = 0;

//     float lSpd = 0;
//     float rSpd = 0;

//     while(!movePID.isSettled()) {
//         float cur = avgDist();
//         float output = movePID.calculateOutput(cur);


//         if(timer < time) {
//             //straight
//             kT = -inertial.get_rotation() * thetaPID;
//         }
//         else {
//             if(ang > 0) {
//                 lSpd+=10;
//             }
//             else {
//                 rSpd+=10;
//             }
//         //     //turn
//         //     kT = (ang-inertial.get_rotation()) * thetaPID;
//         }

//         // if(ang > 0) {
//         //     LeftDT.move_voltage(output + kT);
//         //     RightDT.move_voltage(output);
//         // }
//         // else {
//         //     LeftDT.move_voltage(output);
//         //     RightDT.move_voltage(output - kT);
//         // }
//         LeftDT.move_voltage(0.5*output + kT + lSpd);
//         RightDT.move_voltage(0.5*output - kT + rSpd);

//         timer += 10;
//         pros::delay(10);
//     }
    LeftDT.move_voltage(0);
    RightDT.move_voltage(0);
    LeftDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    RightDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    stopMotors();
}

//pivot func

void pivot(double angle) {
    //theta = total turn
    //angle = target angle (0 to 360 degrees)
    //startAngle = current angle of robot -inf < degrees < inf
    double theta = 0;
    double startAngle = inertial.get_rotation();

    // startAngle mod stuff - getting relative to (0 to 360 degrees)
    int quotient;
    if(startAngle < 0){
        quotient = -1 * int(startAngle / 360) + 1;
        startAngle = startAngle + quotient * 360;
    }
    else{
        quotient = int(startAngle / 360);
        startAngle = startAngle - quotient * 360;
    }

    // target angle fr
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

void moveToPoint(Waypoint target, bool isReversed) {
    float currentX, currentY, currentTheta;
    float distanceToTarget, targetTheta;
    float lateralOutput, angularOutput;

    do {
        Waypoint currentPos = getCurrentPose();

        currentX = currentPos.x;
        currentY = currentPos.y;
        currentTheta = currentPos.theta;

        if (isReversed) {
            currentTheta -= 180;
        }

        // Normalize the current angle
        while (currentTheta > 180) currentTheta -= 360;
        while (currentTheta < -180) currentTheta += 360;

        // Calculate distance and heading to the target
        distanceToTarget = distance(currentPos, target);
        targetTheta = angle(currentPos, target);

        //Set targets
        movePID.setTarget(distanceToTarget);
        turnPID.setTarget(targetTheta);

        // Calculate heading error
        float headingError = targetTheta - currentTheta;

        while (headingError > 180) headingError -= 360;
        while (headingError < -180) headingError += 360;

        // PID calculations
        // lateralOutput = movePID.calculateOutput(distanceToTarget) * cos(headingError * degToRad);
        // angularOutput = turnPID.calculateOutput(headingError);

        lateralOutput = 1100 * distanceToTarget * cos(headingError * degToRad);
        angularOutput = 300 * headingError;

        if (distanceToTarget < 4) {
            angularOutput = 0; // Stop angular adjustment if too close
        }

        lcd::print(5, "Lat: %f\n", lateralOutput);
        lcd::print(6, "Ang: %f\n", angularOutput);

        arcade(lateralOutput, angularOutput); // Robot movement

        pros::delay(10);
    } while (!movePID.isSettled());

    arcade(0, 0); // Stop the robot
}

void goToPoint(float tX, float tY) {
    Waypoint target = Waypoint(tX, tY);
    Waypoint currentPos = getCurrentPose();

    float distanceToTarget = distance(currentPos, target);
    
    lcd::print(4, "distance to target: %f\n", distanceToTarget);
    float targetTheta = angle(currentPos, target);

    pivot(targetTheta);
    driveDist(distanceToTarget);
}


// ICR = instantaneous center of rotation
// angle = angle bot is turning at (in degrees) 
// negative is rotating counterclockwise? positive is rotating clockwise? 
// maxVel = the max velocity defaulting to 90 inches per second. this will need to be tuned depending on the turn.
// direction: true = forward, false = backwards
// left: if the ICR is to the left or right of the robot.
void swervePoint(bool left, float ICR_x, float ICR_y, float angle, bool direction, float maxVel){
    // botTHICKNESS
    float halfWidth = botTHICKNESS / 2;

    Waypoint currentPos = getCurrentPose();
    float currentX = getLeft();
    float currentY = getRight();


    float R = sqrt((ICR_x - currentX) * (ICR_x - currentX) + (ICR_y - currentY) * (ICR_y - currentY));

    
    lcd::print(4, "R: %f\n", R);

    float w = maxVel / R;
    float totalTime = 2 * PI / w; // Time to complete 1 rotation about ICR
    
    lcd::print(7, "total time: %f\n", totalTime);

    // Calculate the radius from the center of rotation to each wheel
    float radiusL;
    float radiusR;
    if(left == true){
        radiusL = R - halfWidth;
        radiusR = R + halfWidth;
    }
    else{
        radiusL = R + halfWidth;
        radiusR = R - halfWidth;
    }

    float rVel = w * radiusR;
    float lVel = w * radiusL;


    float time = totalTime * angle / 360;
    
    float dirConst = 0;

    if(direction == true){
        dirConst = 1;
    }
    else{
        dirConst = -1;
    }

    LeftDT.move_voltage(12000 / maxVel * lVel * dirConst);
    RightDT.move_voltage(12000 / maxVel * rVel * dirConst);
    
    lcd::print(5, "left: %f\n", 12000 / maxVel * lVel * dirConst);
    
    lcd::print(6, "right: %f\n", 12000 / maxVel * rVel * dirConst);
    pros::delay(time * 1000); //undershoot probably
    stopMotors();
    
}


void swing(bool left, float d, float otherMotor) {

    set();
   
    float cur = avgDist();

    //Drive forward/backward
    movePID.setTarget(d + cur);

    if(left) {
        while(!movePID.isSettled()) {
            cur = getLeft();
            float o = movePID.calculateOutput(cur);
            LeftDT.move_voltage(o);
            RightDT.move_voltage(otherMotor*o);
            pros::delay(10);
        }
    }
    else {
        while(!movePID.isSettled()) {
            cur = getRight();
            float o = movePID.calculateOutput(cur);
            LeftDT.move_voltage(otherMotor*o);
            RightDT.move_voltage(o);
            pros::delay(10);
        }
    }

    
    stopMotors();

    controller.rumble(".");


}