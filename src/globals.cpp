#include "main.h"
#include "globals.h"

Motor FL(19, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor ML(20, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor BL(6, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor FR(11, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor MR(13, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor BR(15, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //

Motor Cata1(3, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);
Motor Cata2(8, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

IMU inertial(0);
Rotation leftRot(0);
Rotation rightRot(0);

//think these are ok
ADIDigitalOut wings('A');
ADIDigitalOut intake('H');

Motor_Group LeftDT ({FL, ML, BL});
Motor_Group RightDT ({FR, MR, BR});
Motor_Group cata ({Cata1, Cata2});




