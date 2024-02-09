#include "main.h"
#include "globals.h"

Motor FL(6, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor ML(7, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor BL(8, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor FR(3, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor MR(2, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor BR(4, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //

Motor cata(9, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);

Motor intake(5, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

IMU inertial(11); //!!!
Rotation leftRot(20);
Rotation rightRot(1);

//think these are ok
ADIDigitalOut wings('F');
ADIDigitalOut backWing('G');
ADIDigitalOut intakePiston('H');


Motor_Group LeftDT ({FL, ML, BL});
Motor_Group RightDT ({FR, MR, BR});




