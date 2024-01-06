#include "routes.h"
#include "autofuncs.h"
#include <cmath>

using namespace pros;

void defenseRoute() {

    // start alliance triball to the left of bot

    // drive straight for right triball, intake. 
    // swerve to face left triball while opening wings
    // full speed forward and push left triball over
    // outtake and drop right triball over

    // turn like 180 to face alliance triball 
    // drive to alliance triball, intake
    // turn to face goal, release intake, full power forward

    // ------------------------------

    //Start 45 degree diagonal w/ Alliance triball touching bot
    // driveDist(-40, -40, 0.8, 0);
    // pros::delay(200);
    // turn(45);
    // LeftDT.move_voltage(-12000);
    // RightDT.move_voltage(-12000);
    // pros::delay(700);
    // stopMotors();
    // pros::delay(50);
    // driveDist(17.5, 17.5, 1, 0);
    // pros::delay(50);
    // turn(-45);
    // wings.set_value(1);
    // driveDist(22, 22, 1, 0);
    // turn(-90);
    // wings.set_value(0);

    // driveDist(34, 34, 1, 0);
    // turn(-135);
    // wings.set_value(1);
    
    // driveDist(-35, -35, 1, 0);
    // turn(-20);

}

void offensiveSneak() {
    intake.move_voltage(12000);
    LeftDT.move_voltage(12000 * 3/6);
    RightDT.move_voltage(12000 * 3/6);
    pros::delay(100);
    stopMotors();
    driveDist(-48);

    turn(-45);
    pros::delay(50);

    driveTime(-530);

    turn(-45);
    pros::delay(50);

    driveTime(-700);
    // +1 Balls

    driveTime(200);
    pros::delay(50);
    turn(180);

    intake.move_voltage(-12000);

    pros::delay(200);
    driveTime(500);
    // +1 Ball
    intake.move_voltage(0);

    /*
    past 8
    */

    driveDist(-17); //this point on the values in yellow (aka the #'s need to be tested)
    turn(-70);
    intake.move_voltage(12000);

    driveDist(67);


    turn(135);
    intake.move_voltage(-12000);

    //**
    LeftDT.move_voltage(12000 * 4.5/6);
    RightDT.move_voltage(12000 * 4.5/6);
    pros::delay(450);
    stopMotors();
    pros::delay(250);
    LeftDT.move_voltage(-12000 * 4.5/6);
    RightDT.move_voltage(-12000 * 4.5/6);
    pros::delay(150);
    stopMotors();
    //**

    turn(-90);
    
    LeftDT.move_voltage(8000);
    RightDT.move_voltage(8000);
    pros::delay(300);
    LeftDT.move_voltage(2000);
    RightDT.move_voltage(12000);
    pros::delay(350);
    stopMotors();
    
    // intake.set_value(1);
    
    turn(180);
    pros::delay(150);

    wings.set_value(1);

    intake.move_voltage(-12000);
    
    driveTime(2000);
    wings.set_value(0);

    controller.rumble("...");
}

void skillz() {
    const int MATCHLOAD_TIME = 0; //in ms

    driveTime(-400);   
    turn(45);
    driveTime(-700);

    driveDist(15);
    turn(-95);

    LeftDT.move_voltage(-6000);
    RightDT.move_voltage(-6000);
    pros::delay(600);
    stopMotors();

    long long timer = 0;
    while(timer < MATCHLOAD_TIME) {
        cata.move_voltage(12000 * 0.80);

        timer += 10;
        pros::delay(10);
    }



}

void offensiveRush() {
    
}


//			arc(1000, 650, 1); this might work