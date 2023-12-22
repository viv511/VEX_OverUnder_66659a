#include "cata.h"

using namespace pros;

pros::Mutex cataReady;
bool cata_ready = false;

void setState(bool state) {
    cataReady.take();
    cata_ready = state;
    cataReady.give();
}

bool getState() {
    bool tempState;
    cataReady.take();
    tempState = cata_ready;
    cataReady.give();
    return tempState;
}


void cataCtrl() {
    cata.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    while(true) {




        pros::delay(10);
    }


}