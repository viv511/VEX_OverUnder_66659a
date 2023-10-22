#include "routes.h"
#include "autofuncs.h"
#include <cmath>

using namespace pros;

void defenseRoute() {
    //Start 45 degree diagonal w/ Alliance triball touching bot
    driveDist(-40, -40, 0.8, 0);
    pros::delay(200);
    turn(45);
    LeftDT.move_voltage(-12000);
    RightDT.move_voltage(-12000);
    pros::delay(700);
    stopMotors();
    pros::delay(50);
    driveDist(17.5, 17.5, 1, 0);
    pros::delay(50);
    turn(-45);
    wings.set_value(1);
    driveDist(22, 22, 1, 0);
    turn(-90);
    wings.set_value(0);

    driveDist(34, 34, 1, 0);
    turn(-135);
    wings.set_value(1);
    
    driveDist(-35, -35, 1, 0);
    turn(-20);

}

void offensiveSneak() {
    intake.set_value(1);
    driveDist(-48, -48, 1, 0);
    turn(-45);
    driveDist(-35, -35, 1, 0);
    turn(-45);
    LeftDT.move_voltage(-12000);
    RightDT.move_voltage(-12000);
    pros::delay(500);
    stopMotors();
    driveDist(10, 10, 1, 0);
    turn(180);
    intake.set_value(0);
    pros::delay(200);
    LeftDT.move_voltage(12000);
    RightDT.move_voltage(12000);
    pros::delay(500);
    stopMotors();
    
    driveDist(-15, -15, 1, 0);
    turn(-90);

    //Wallbang
    LeftDT.move_voltage(-12000);
    RightDT.move_voltage(-12000);
    pros::delay(300);
    stopMotors();

    driveDist(30, 30, 1, 0);
    turn(25);
    pros::delay(10);
    driveDist(45, 45, 1, 0);

    intake.set_value(1);
    turn(140);
    intake.set_value(0);
    LeftDT.move_voltage(12000);
    RightDT.move_voltage(12000);
    pros::delay(2000);
    stopMotors();
}

