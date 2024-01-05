#include "routes.h"
#include "autofuncs.h"
#include <cmath>

using namespace pros;

void defenseRoute() {
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
    driveTime(300);
    driveDist(-53);

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
    driveDist(-25.5);
    turn(45);
    driveTime(-700);

    driveDist(20);
    turn(-90);

    LeftDT.move_voltage(4000);
    RightDT.move_voltage(4000);
    pros::delay(300);
    stopMotors();

}

void offensiveRush() {
    
}


//			arc(1000, 650, 1); this might work