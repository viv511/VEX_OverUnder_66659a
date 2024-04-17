#include "routes.h"
#include "autofuncs.h"
#include <cmath>

using namespace pros;

//Route Notes!
//https://docs.google.com/document/d/15k4KlxSqy_QPsQVgDtnAq57L73HCmbvh--U-k1gcZbc/edit?tab=t.0#heading=h.u6psvttskpq1

void exampleRoute() {
    //Some help for you all~~

    //**** Driving Forwards! ****//
        //driveDist() == Driving Forward or Backwards using PID!
        //Two parameters, first is required distance, second is optional limit (# from 0 - 1 describing power)

        //EXAMPLE: driveDist(12, 0.5); //Drive 12 inches forward at 50% power
        //EXAMPLE: driveDist(-23); //Drive 23 inches backwards

        //driveTime() == Driving Forward or Backwards using time! [not recommended]
        //One parameter, time in milliseconds
        //Useful for when we want to push a ball into the goal at a fast speed in a small amount of time

        //EXAMPLE: driveTime(-700); //Drive backwards for 700 milliseconds
        //EXAMPLE: driveTime(1300); //Drive forwards for 1300 milliseconds

    //**** Turning! ****//
        //When the robot begins at the start of the route, the direction the front of the robot is facing is considered 0.
        //Turning right is considered positive, turning left is considered negative.
        //Any possible angle is bounded by -180 to 180 degrees.

        //This means our turning is not RELATIVE, but ABSOLUTE to the direction the robot started at.
        //Let theta = the direction the robot started at.

        //EXAMPLE: pivot(90); //Turn 90 degrees to the right of theta
        //EXAMPLE: pivot(-75); //Turn 75 degrees to the left of theta

    //**** Intake! ****//
        //intake.move_voltage(12000) == Intake Forward (take in triball)
        //intake.move_voltage(-12000) == Intake Backward (push out triball)
        //intake.move_voltage(0) == Stop Intake

        //NOTE: Make sure to stop intake after you begin to turn it on.
       
        //EXAMPLE:
        //intake.move_voltage(12000);
        //driveDist(12);
        //intake.move_voltage(0);
        //>> This will intake a triball after driving forward 12 inches, then stop the intake

    //**** Wings! ****//
        //wings.set_value(true) == Front Wings Out
        //wings.set_value(false) == Front Wings In
        //backWing.set_value(true) == Back Wings Out
        //backWing.set_value(false) == Back Wings In

        //EXAMPLE:
        //wings.set_value(true);
        //driveDist(12);
        //wings.set_value(false);
        //>> This will extend the front wings, drive forward 12 inches, then retract the front wings

        //Wings will stay open until you tell them to close by setting the value to false.
}

void defensive_wp(){
    backWing.set_value(true);
    driveDist(4);
    pivot(-46);
    backWing.set_value(false);
    driveDist(36);
    intake.move_voltage(-12000);
    delay(1000);
    intake.move_voltage(0);
    
 

}