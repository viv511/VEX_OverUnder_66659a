#include "pid.h"
#include <cmath>

using namespace pros;

const float INTEGRAL_TURN_THRESHOLD = 3; 
const float INTEGRAL_DRIVE_THRESHOLD = -1000; //Disabled with any negative number
const int PID_DELAY_TIME = 15; 

PID::PID() {
    resetPlease();
}

PID::PID(float kP, float kI, float kD, bool isTurnPID, float sErr, float sExit, float lErr, float lExit, float maxAllowedTime) {
    resetPlease();
    setConstants(kP, kI, kD);
    setType(isTurnPID);
    setExitConditions(sErr, sExit, lErr, lExit, maxAllowedTime);
}

PID::PID(float kP, float kD, bool isTurnPID, float sErr, float sExit, float lErr, float lExit, float maxAllowedTime) {
    resetPlease();
    setConstants(kP, 0, kD);
    setType(isTurnPID);
    setExitConditions(sErr, sExit, lErr, lExit, maxAllowedTime);
}

//best ref fr
void PID::resetPlease() {
    this->target = 0;
    this->isTurn = true;
    this->error = 0;
    this->previousError = 0;
    this->output = 0;
    this->derivative = 0;
    this->integral = 0;
    resetConstants();
}

void PID::resetConstants() {
    this->kP = 0;
    this->kI = 0;
    this->kD = 0;

    this->smallTimeCounter = 0;
    this->largeTimeCounter = 0;
    this->maxCounter = 0;
}

void PID::setConstants(float p, float i, float d) {
    this->kP = p;
    this->kI = i;
    this->kD = d;
}

float PID::getTarget() {
    return this->target;
}

void PID::setTarget(float t) {
    this->target = t;
    this->smallTimeCounter = 0;
    this->largeTimeCounter = 0;
    this->maxCounter = 0;
}

void PID::setType(bool turn) {
    this->isTurn = turn;
}

void PID::setExitConditions(float sErr, float sExit, float lErr, float lExit, float maxOut) {
    this->smallErr = sErr;
    this->smallExit = sExit;
    this->largeErr = lErr;
    this->largeExit = lExit;
    this->maxTime = maxOut;
}

float PID::calculateOutput(float current) {
    this->error = this->target - current;
    this->derivative = this->error - this->previousError;

    if(this->kI == 0) {
        this->integral = 0;
    }
    else {
        if(isTurn) {
            if(fabs(this->error) < INTEGRAL_TURN_THRESHOLD) {
                this->integral = this->integral + this->error;
            }
        }
        else {
            if(fabs(this->error) < INTEGRAL_DRIVE_THRESHOLD) {
                this->integral = this->integral + this->error;
            }
        }

        if(numbersign(this->error) != numbersign(this->previousError)) {
            this->integral = 0;
        }
    }

    this->output = (this->kP * this->error) + (this->kI*this->integral) + (this->kD * this->derivative);
    this->previousError = this->error;

    updateTimers();

    return this->output;
}

void PID::updateTimers() {
    if(fabs(this->error) < this->smallErr) {
        this->smallTimeCounter += PID_DELAY_TIME;
    }
    else if(fabs(this->error) < this->largeErr) {
        this->largeTimeCounter += PID_DELAY_TIME;
    }

    this->maxCounter += PID_DELAY_TIME;
}

bool PID::isSettled() {
    if(smallTimeCounter > smallExit) {
        //In target small threshold for smallExit amount of time
        pros::lcd::print(6, "diff: %f\n", error);
        pros::lcd::print(5, "Exit: %s\n", "SMALL");
        return true;
    }
    else if(largeTimeCounter > largeExit) {
        //In target large threshold for largeExit amount of time
        pros::lcd::print(6, "diff: %f\n", error);
        pros::lcd::print(5, "Exit: %s\n", "LARGE");
        return true;
    }
    else if(maxCounter > maxTime) {
        //took too long
        pros::lcd::print(6, "diff: %f\n", error);
        pros::lcd::print(5, "Exit: %s\n", "MAX");
        return true; 
    }

    //else it hasnt reached yet
    return false;
}

int PID::numbersign(float num) {
	if(num > 0.0) {
		return 1;
	}
	else if(num < 0.0) {
		return -1;
	}
	else {
		return 0;
	}
}