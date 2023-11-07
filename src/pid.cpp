#include "pid.h"
#include <cmath>

using namespace pros;

const float INTEGRAL_TURN_THRESHOLD = 5; 
const float INTEGRAL_DRIVE_THRESHOLD = -1000; //Disabled with any negative number
const int PID_DELAY_TIME = 10; 

const float lin_kP = 850;
const float lin_kD = 100;

PID::PID() {
    resetPlease();
}

PID::PID(bool isTurnPID, float sErr, float sExit, float lErr, float lExit, float maxAllowedTime) {
    resetPlease();
    setType(isTurnPID);
    setExitConditions(sErr, sExit, lErr, lExit, maxAllowedTime);
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
    // if(this->isTurn) {
    //     this->target += inertial.get_rotation();
    // }
    this->smallTimeCounter = 0;
    this->largeTimeCounter = 0;
    this->maxCounter = 0;

    /*
    Figure out the ideal PID constants depending on the target & type of PID

    **** MICHAEL MATOME AND I NEED TO TUNE THIS AREA THE MOST ****
    **** BIG PART OF CODE NOTEBOOK ****

    NOTE: Only when all the PID constants aren't defined and/or set to 0.
    */

    if(this->isTurn == true) {
        turnLookupPID(this->target);
    }
    else {
        setConstants(15*this->target, 0, 100);
        if(this->target >= 35) {
            setConstants(15*this->target, 0, 200);
        }
    }
}

void PID::turnLookupPID(float t) {
    t = fabs(t);

    if(t <= 10) {
        setConstants(450, 2, 200);
    }
    else if(t <= 20) {
        setConstants(250, 0, 50);
    }
    else if(t <= 30) {
        setConstants(150, 1, 40); 
    }
    else if(t <= 45) {
        setConstants(230, 0, t*18);
    }
    else if(t <= 60) {
        setConstants(350, 0, t*15);
    }
    else if(t <= 70) {
        setConstants(330, 0, t*20); 
    }
    else if(t <= 80) {
        setConstants(310, 0, t*25);
    }
    else if(t <= 90) {
        setConstants(325, 0, t*27);
    }
    else if(t <= 105) {
        setConstants(350, 0, t*27);
    }
    else if(t <= 120) {
        setConstants(375, 0, t*24);
    }
    else if(t <= 135) {
        setConstants(400, 0, t*22);
    }
    else if(t <= 150) {
        setConstants(425, 0, t*20);
    }
    else if(t <= 165) {
        setConstants(450, 0, t*19);
    }
    else {
        setConstants(500, 0, t*21.5);
    }
    // controller.rumble(".");
}

void PID::moveLookupPID(float t) {
    setConstants(lin_kP, 0, lin_kD*fabs(t));
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

    }

    if(numbersign(this->error) != numbersign(this->previousError)) {
        this->integral = 0;
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

float PID::autoTuneAngle() {
  bool consistentGood = false;
  int tries = 0;
  float t = this->target;
  
  float pGain = 200;
  float pMin = 0;
  float pMax = 400;

  while (consistentGood == false) { 
    this->resetPlease();
    this->setConstants(pGain, 0, 0);
    this->setTarget(t);

    while (!isSettled()) {
      float o = this->calculateOutput(inertial.get_rotation());
      LeftDT.move_voltage(o);
      RightDT.move_voltage(-o);
      pros::delay(10);
    }

    LeftDT.move_voltage(0);
    RightDT.move_voltage(0);
    LeftDT.brake();
    RightDT.brake();

    if(this->cross > 2) {
        tries = 0;
       
        pMax = pGain;
        pGain = (pGain + pMin) / 2.0;

    }
    else if ((this->error) < 1) {
        tries++;
    }
    else {
        tries = 0;

        if (this->error > 0) {
            pMin = pGain;
            pGain = (pGain + pMax) / 2.0;
        } 
        else if (this->error < 0) {
            pMax = pGain;
            pGain = (pGain + pMin) / 2.0;
        }
    }

    if (tries == 3) {
      return pGain;
    }
    
    std::cout << pGain << "\n";
    pros::delay(10);
  }

  return 100;
}
