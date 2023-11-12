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
    //STEPS FOR NEXT COMP OFFENSIVE ROUTE
    /*
    1. Position same as last time
    2. Intake and drive backwards til barrier
    3. -45 degree turn & drive back a bit
    4. Open Wings -> Spin a bit so the triball goes out -> spin back -> close wings
    5. -45 again, and drive backwards full speed to push both triballs in
    6. turn 180
    7. open intake and push triball in
    8. drive backwards
    9. turn -90 & use fancy "arc" function OR turn angle more than -90 (i.e. -75 ish) that gets you pointed at the triball at the bar
    10. get to that triball and intake it
    11. drive backwards
    12. point towards goal & open intake
    13. drive a little forward not all the way ==> causes triball to roll
    14. turn towards direct middle middle bar triball 
    15. drive forward and close intake on the triball
    16. face OPPOSITE the goal & open wings
    17. push the 2 triballs in
    18. go forward & 180 degrees
    19. Open intake and drive forward forever (we will be cut out by the 15 sec time limit so we want to make sure we drive full force until then)
    
    */

    //   ~~~~~~~~~~~~~~~~~~~~~~~~ Mashpee code ~~~~~~~~~~~~~~~~~~~~~~~~
    // intake.set_value(1);
    // driveDist(-48, -48, 1, 0);
    // turn(-45);
    // driveDist(-35, -35, 1, 0);
    // turn(-45);
    // LeftDT.move_voltage(-12000);
    // RightDT.move_voltage(-12000);
    // pros::delay(500);
    // stopMotors();
    // driveDist(10, 10, 1, 0);
    // turn(180);
    // intake.set_value(0);
    // pros::delay(200);
    // LeftDT.move_voltage(12000);
    // RightDT.move_voltage(12000);
    // pros::delay(500);
    // stopMotors();
    
    // driveDist(-15, -15, 1, 0);
    // turn(-90);

    // //Wallbang
    // LeftDT.move_voltage(-12000);
    // RightDT.move_voltage(-12000);
    // pros::delay(300);
    // stopMotors();

    // driveDist(30, 30, 1, 0);
    // turn(25);
    // pros::delay(10);
    // driveDist(45, 45, 1, 0);

    // intake.set_value(1);
    // turn(140);
    // intake.set_value(0);
    // LeftDT.move_voltage(12000);
    // RightDT.move_voltage(12000);
    // pros::delay(2000);
    // stopMotors();
}

void offensiveGreed() {
    //five triball route
}