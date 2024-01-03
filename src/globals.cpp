#include "main.h"
#include "globals.h"

Motor FL(20, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor ML(18, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor BL(13, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor FR(19, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor MR(16, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor BR(14, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //

Motor cata(17, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);

Motor intake(1, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

IMU inertial(7); //!!!
Rotation leftRot(11);
Rotation rightRot(12);

//think these are ok
ADIDigitalOut wings('A');
ADIDigitalOut elev('C');
ADIDigitalOut blocker('B');


Motor_Group LeftDT ({FL, ML, BL});
Motor_Group RightDT ({FR, MR, BR});




