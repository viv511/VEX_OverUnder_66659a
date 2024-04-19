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

void safe_offensive() {
   intake.move_voltage(12000);
   driveTime(150);
   delay(500);
   intake.move_voltage(0);
   driveDist(-33);
   pivot(-45);
   backWing.set_value(true);
   driveDist(-27);
   delay(100);
   backWing.set_value(false);
   pivot(-83);
   pivot(100);
   intake.move_voltage(-12000);
   driveTime(-50);
   delay(50);
   intake.move_voltage(0);
   driveTime(400);
   driveTime(-260);
   intake.move_voltage(-12000);
   driveTime(500);
   delay(500);
   driveDist(-20);
   driveDist(8);

   pivot(15);
   intake.move_voltage(12000);
   driveDist(51);
   intake.move_voltage(0);
   pivot(160);
   intake.move_voltage(-12000);
   driveTime(15000);
   driveDist(-5);
}


void offensive_rush(){
   intake.move_voltage(120000);
   driveDist(66);
   delay(100);
   pivot(45);
   intake.move_voltage(-120000);
   wings.set_value(true);
   intake.move_voltage(0);
   driveDist(48);
   driveDist(-48);
   driveDist(48);
   driveDist(-48);
   driveTime(-12000);

   pivot(180);
   intake.move_voltage(120000);
   driveDist(60);

   intake.move_voltage(-120000);
   pivot(180);
   driveDist(60);
   intake.move_voltage(0);
   driveTime(15000);
   driveTime(-10000);
   driveDist(-5);

   pivot(45);
   intake.move_voltage(120000);
   driveDist(45);


}


