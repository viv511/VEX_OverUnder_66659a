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
    pivot(-100);
    delay(500);
    pivot(-35);
    backWing.set_value(false);
    intake.move_voltage(-12000);
    driveDist(40);
    pivot(-46);
    delay(1000);
    intake.move_voltage(0);

}

void safe_offensive() {
    //intake is gud
   
    intake.move_voltage(12000);
    driveTime(180);
    delay(750);
    intake.move_voltage(0);

    //bot goes to curve
    driveDist(-32.4);
    pivot(-32.5);
    
    backWing.set_value(true);
    driveDist(-8);
    pivot(-45);

    //bot get ball333
    driveDist(-15);
    delay(125);
    
    pivot(-78);
    backWing.set_value(false);
    driveTime(-600);
    driveDist(12);
    pivot(100);
    intake.move_voltage(-12000);
    
   
    driveTime(500);
    delay(100);
    intake.move_voltage(0);
    driveDist(-15);
    //3 triball go bye bye

    pivot(17.5);
    intake.move_voltage(12000);
    driveDist(52);
    delay(200);
    intake.move_voltage(0);
    pivot(150);
    intake.move_voltage(-12000);
    delay(150);
    driveTime(1000);
    driveTime(-400);
}

void defensive_rush() {
    intake.move_voltage(12000);
    driveDist(49);
    intake.move_voltage(0);
    driveDist(-48);
    pivot(93);
    intake.move_voltage(-12000);
    delay(150);
    intake.move_voltage(0);
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-3500);
    delay(230);
    LeftDT.move_voltage(-4700);
    delay(140);
    LeftDT.move_voltage(-6100);
    delay(470);
    LeftDT.move_voltage(-12000);
    delay(150);
    stopMotors();
    pivot(157);
    driveTime(80);
    RightDT.move_voltage(12000);
    LeftDT.move_voltage(4700);
    delay(240);
    backWing.set_value(true);
    delay(530);
    backWing.set_value(false);
    LeftDT.move_voltage(12000);
    delay(100);

    pivot(72);
    intake.move_voltage(-12000);
    driveTime(630);
    intake.move_voltage(0);


}


void offensive_rush(){
   intake.move_voltage(12000);
   driveDist(66);
   delay(100);
   pivot(45);
   intake.move_voltage(-12000);
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


