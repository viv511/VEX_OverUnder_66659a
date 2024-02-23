#include "routes.h"
#include "autofuncs.h"
#include <cmath>

using namespace pros;


void defensiveRush(){
    wings.set_value(1);

    delay(350);
    wings.set_value(0);
    intake.move_velocity(600);
    driveDist(52);
   
    
    // drive backwards  and open wings
    driveDist(-11); // TUNE PLS VIVEKKKK

    wings.set_value(1);
    intake.move_velocity(0);


    // drift
    LeftDT.move_voltage(12000);
    delay(320);
    LeftDT.move_voltage(0);

    // delay(50);

    // correction
    // turn(73, true);

    // drive forward to guarantee ball gets pushed over.
    LeftDT.move_voltage(12000);
    RightDT.move_voltage(12000);
    delay(340);
   
    LeftDT.move_voltage(3000);
    RightDT.move_voltage(3000);
    delay(100);
    LeftDT.move_voltage(0);
    RightDT.move_voltage(0);
    delay(20);
    
    // go back and set bot up to score the preload.
    driveDist(-9); //TUNE THIS 
    wings.set_value(false);



    // expected_rotation = 0;
    // turn(-55);
    pivot(17);

    driveDist(-58);
    backWing.set_value(1);
    delay(120);
    turn(-275);

    backWing.set_value(0);

    // turn(12);
    delay(50);
    LeftDT.move_voltage(12000);
    RightDT.move_voltage(12000);

    delay(70);

    LeftDT.move_voltage(-4850);
    RightDT.move_voltage(-12000);
    delay(550);
    
    LeftDT.move_voltage(-12000);
    delay(330);

    // delay(30);

    LeftDT.move_voltage(0);
    RightDT.move_voltage(0);
    delay(100);

    RightDT.move_voltage(12000);
    LeftDT.move_voltage(12000);
    delay(80);
    LeftDT.move_voltage(4900);
    delay(740);
    LeftDT.move_voltage(0);
    RightDT.move_voltage(0);

    pivot(72);

    // touch bar and outtake
    intake.move_velocity(-600);
    driveTime(270);
    RightDT.move_voltage(10000);
    LeftDT.move_voltage(10000);
    delay(200);
    RightDT.move_voltage(5500);
    LeftDT.move_voltage(5500);
    delay(250);
    RightDT.move_voltage(0);
    LeftDT.move_voltage(0);
}


void defenseRoute() {

    //start with preload outside intake to the left
    intake.move_voltage(12000);
    driveDist(49.5);
    wings.set_value(1);
    driveDist(-10);
    turn(77);
    driveDist(20);
    intake.move_voltage(-12000);
    pros::delay(250);
    wings.set_value(0);
    driveDist(-10);
    swing(true, -75, 0.05);

    intake.move_voltage(12000);
    driveDist(30);
    swing(false, -60, 0);
    intake.move_voltage(-12000);
    pros::delay(100);
    driveTime(500);

    //swing backwards and clockwise idk what the input is (around 75 degrees)
    //swing()
    driveTime(-100);

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
    //intakePiston.set_value(true);
    pros::delay(100);
    LeftDT.move_voltage(12000 * 3/6);
    RightDT.move_voltage(12000 * 3/6);
    pros::delay(100);
    stopMotors();

    //step 2: drive backwards around corner to score triball #1
    pros::delay(200);
    driveTime(-460);
	RightDT.move_voltage(-4800);
    LeftDT.move_voltage(-12000);
    backWing.set_value(1);
    delay(230);
    delay(300);
    stopMotors();
    backWing.set_value(0);

    driveTime(-450);
    // +1 Balls

    //step 3: turn around and release intake to score triball #2
    driveTime(200);
    pros::delay(50);
    pivot(90);
    intake.move_voltage(-12000);
    driveTime(800); // make it back up and push again if not work
    pros::delay(50);
    driveTime(-300);
    pros::delay(50);
    pivot(90);
    pros::delay(50);
    // driveTime (400);
    // +1 Ball

    //step 4: turn around and drive forwards to pick up triball #3 + throw it in front of goal
    intake.move_voltage(0);
    driveTime(-150);
    pivot(25); //important turn to face triball #3
    intake.move_voltage(12000);
    driveDist(67);
    pivot(155);
    intake.move_voltage(-12000);
    driveTime(200);
    driveTime(-150);

    //step 5: swing around to triball #4, intake and release while opening wings for triball #5
    pivot(52);
    intake.move_voltage(12000);
    driveTime(500);
    pivot(180);
    wings.set_value(1); 
    intake.move_voltage(-12000);
    driveTime(2000);
    
    controller.rumble("...");
}

void startOfSkills() { 
    
    // Push balls in. 
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-6000);
    delay(550);
    LeftDT.move_voltage(-12000);
    delay(220);
    pivot(45);
    delay(50);

    // Set up for matchloading
    RightDT.move_voltage(12000);
    LeftDT.move_voltage(4000);
    delay(300);
    pivot(-63);
    LeftDT.move_voltage(-5000);
    RightDT.move_voltage(-5000);
    pros::delay(20);
    pivot(-63);
    stopMotors();
}

void betterskillz(){
    startOfSkills();

    backWing.set_value(1);

    // matchloading
    const int MATCHLOAD_TIME = 27500; //in ms (27500)
    long long timer = 0;
    while(timer < MATCHLOAD_TIME) {
        cata.move_voltage(12000);
        intake.move_voltage(-12000);
        timer += 10;
        pros::delay(10);
    }
    cata.move_voltage(0);
    intake.move_voltage(0);
    backWing.set_value(0);
    stopMotors();
    pros::delay(50);


    // Go to alley
    pivot(180);
    driveTime(-165);
    RightDT.move_voltage(-6300);
    LeftDT.move_voltage(-12000);
    delay(390);
    stopMotors();
    pivot(136.7);

    // push through alley
    cata.move_voltage(12000* 0.67);
    driveDist(-78);
    cata.move_voltage(0);
    pivot(133);
    delay(150);
    


    //side push w/ swerve
    driveTime(-110);
	RightDT.move_voltage(-4900);
    LeftDT.move_voltage(-12000);
    delay(740);
    stopMotors();
    pivot(45);
    driveTime(-400);
    delay(50);

    //second push
    driveTime(270);
    delay(50);
    driveTime(-410);
    delay(50);
    pivot(45);

    //third push
    // driveTime(320);
    //  delay(50);
    // driveTime(-450);
    // delay(50);
    // pivot(45);


    // set up for push from front
    driveDist(11);
    pivot(145);
    driveDist(51);
    pivot(45);
    driveTime(-135);
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-3000);
	delay(485);
    backWing.set_value(1);
    stopMotors();
    delay(100);
    pivot(135);
    delay(50);

    // front push
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-12000);
    delay(1200);
    pivot(135);
    delay(200);

    // set up for push
    backWing.set_value(0);
    driveDist(30);
    delay(200);
    pivot(45);
    driveTime(-85);
    LeftDT.move_voltage(-3000);
    RightDT.move_voltage(-12000);
	delay(485);
    backWing.set_value(1);

    // 2nd push
    stopMotors();
    delay(50);
    pivot(135);
    delay(50);
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-12000);
    delay(700);
    pivot(135);
    backWing.set_value(0);
    delay(200);

    // push from other side
    driveDist(26);
    delay(20);
    pivot(70);
    driveDist(-42);
    pivot(45);
    driveDist(-15);
    pivot(135);
    driveTime(-100);
    backWing.set_value(1);
    pivot(180);
    driveTime(-180);
    backWing.set_value(0);
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-5800);
    delay(550);
    LeftDT.move_voltage(-12000);
    delay(200);
    stopMotors();
    

    // push more
    driveTime(320);
    delay(50);
    driveTime(-400);
    delay(50);
    driveTime(150);
    
    


}

void skillz3(){

   RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-6000);
    delay(550);
    LeftDT.move_voltage(-12000);
    delay(270);
    pivot(45);
    delay(50);
    RightDT.move_voltage(12000);
    LeftDT.move_voltage(4000);
    delay(300);
    pivot(-65);
    LeftDT.move_voltage(-6000);
    RightDT.move_voltage(-6000);
    pros::delay(100);
    stopMotors();
    backWing.set_value(1);

    // backWing.set_value(1);
    const int MATCHLOAD_TIME = 27000; //in ms (30000)
    long long timer = 0;
    while(timer < MATCHLOAD_TIME) {
        cata.move_voltage(12000);
        timer += 10;
        pros::delay(10);
    }
    cata.move_voltage(0);
    stopMotors();
    pros::delay(50);

    // swerve to alley
    driveTime(50);
    backWing.set_value(0);
    pivot(160);
    driveTime(-550);

    pivot(180);
    driveTime(80);
	RightDT.move_voltage(12000);
    LeftDT.move_voltage(4900);
    delay(750);
    stopMotors();
    delay(50);
    pivot(87.5);

    //drive across field through alley
    driveDist(81);
    pivot(-90);
    delay(150);

    // swerve for side push
    driveTime(-230);
	RightDT.move_voltage(-4900);
    LeftDT.move_voltage(-12000);
    delay(740);
    stopMotors();
    pivot(180);
    driveTime(-400);

    //second push
    driveTime(250);
    delay(50);
    driveTime(-400);
    delay(50);
   


    // go push from front
    driveDist(11);
    pivot(-80);
    driveDist(45);
    wings.set_value(1);
    pivot(0);
    driveTime(105);
    RightDT.move_voltage(3000);
    LeftDT.move_voltage(12000);
	delay(485);
    pivot(90);
    delay(50);
    driveTime(700);
    pivot(90);
    delay(200);

    // #2 push from front
    wings.set_value(0);
    driveDist(-28);
    delay(200);
    pivot(0);
    wings.set_value(1);
    LeftDT.move_voltage(12000);
    RightDT.move_voltage(3000);
	delay(485);
    pivot(90);
    driveTime(700);
    wings.set_value(0);
    delay(200);

    // push from other side
    driveDist(-26);
    delay(20);
    pivot(-140);
    driveDist(-50);
    pivot(-45);
    RightDT.move_voltage(-12000);
    LeftDT.move_voltage(-7500);
    delay(400);
    LeftDT.move_voltage(-12000);
    delay(200);
    stopMotors();
    
    
    

}

void offensiveRush() {
    //Reset: Facing the middle left triball, preload is next to wings

    //Pop wings and drive forward towards mid triball and intake on
    //Pivot towards goal, open wings, outtake, and drive forward
    //Backward Arc with left side ==> Either PID or Time based
    //Go forward, intake on towards back left triball
    
    
}