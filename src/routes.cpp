#include "routes.h"
#include "autofuncs.h"
#include <cmath>

using namespace pros;

void defenseRoute() {
    //Start 45 degree diagonal w/ Alliance triball touching bot
    driveDist(-15, -15, 1, 0);
    turn(45);
    driveDist(-18, -18, 1, 0);
    

}

void offensiveSneak() {
    //Start near elevation facing triball w/ alliance triball touching bot
    driveDist(27, 27, 1, 0);
    intake.set_value(1);
    driveDist(-33, -33, 1, 0);
    turn(-45);
    driveDist(-45, -45, 1, 0);
    turn(-45);
    driveDist(-10, -10, 1, 0);
    driveDist(10, 10, 1, 0);
    turn(180);
    intake.set_value(0);
    driveDist(10, 10, 1, 0);

    driveDist(-11, -11, 1, 0);
    turn(-45);
    driveDist(65, 65, 1, 0);
    turn(45);
    intake.set_value(1);
    turn(-90);
    driveDist(65, 65, 1, 0);
    //Forward
    //Intake open
    //Score

    //********* goal point ******
}

void offensiveRush() {
    //Start 45 degrees near the other matchload
    //Go forward and intake triball in top left
    //face opponents goal, back towards our goal
    //open wings
    //push all the way back and score triball
    //180 degree trun and score other triball
    
    //??????
}
