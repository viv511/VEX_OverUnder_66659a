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

    //step 1: drive forward a bit and intake
    intake.move_voltage(12000);
    LeftDT.move_voltage(12000 * 3/6);
    RightDT.move_voltage(12000 * 3/6);
    pros::delay(100);
    stopMotors();

    //step 2: drive backwards around corner to score triball #1
    driveTime(-650);
    turn(-45);
    pros::delay(50);
    driveTime(-530);
    turn(-45);
    pros::delay(50);
    driveTime(-700);
    // +1 Balls

    //step 3: turn around and release intake to score triball #2
    driveTime(200);
    pros::delay(50);
    turn(180);
    intake.move_voltage(-12000);
    pros::delay(200);
    driveTime(500);
    // +1 Ball

    //step 4: turn around and drive forwards to pick up triball #3 + throw it in front of goal
    intake.move_voltage(0);
    driveDist(-17); 
    turn(-75); //important turn to face triball #3
    intake.move_voltage(12000);
    driveDist(67);
    turn(135);
    intake.move_voltage(-12000);
    driveTime(150);
    driveTime(-150);

    //step 5: swing around to triball #4, intake and release while opening wings for triball #5
    turn(-65);
    intake.move_voltage(12000);
    driveTime(200);
    swing(false, 40, 0.15);
    driveTime(-150);
    turn(180);
    wings.set_value(1);
    intake.move_voltage(-12000);
    driveTime(2000);
    
    controller.rumble("...");
}

void skillz() {
    const int MATCHLOAD_TIME = 0; //in ms

    driveTime(-440);   
    turn(45);
    driveTime(-700);
    swing(false, 37.69, 0);
    driveDist(-30);
    turn(10);

    long long timer = 0;
    while(timer < MATCHLOAD_TIME) {
        cata.move_voltage(12000 * 0.80);

        timer += 10;
        pros::delay(10);
    }
    cata.move_voltage(0);

    swing(true, 30, 0);
    driveDist(20);
    turn(-45);
    driveDist(74);
    turn(-90);
    driveDist(20);
    turn(-60);
    driveDist(35);
    turn(150);
    wings.set_value(1);
    driveTime(1000);
    pros::delay(200);
    driveDist(-30);
    turn(-45);
    driveTime(2000);
}

void offensiveRush() {
    
}


//			arc(1000, 650, 1); this might work