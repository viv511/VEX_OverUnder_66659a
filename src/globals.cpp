#include "main.h"
#include "globals.h"

Motor FL(20, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor ML(3, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor BL(10, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); //
Motor FR(15, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor MR(17, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //
Motor BR(16, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); //

Motor Cata1(7, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES);
Motor Cata2(12, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);

Controller controller(E_CONTROLLER_MASTER);

IMU inertial(2);
Rotation leftRot(18);
Rotation rightRot(11);

//think these are ok
ADIDigitalOut wings('A');
ADIDigitalOut intake('H');

Motor_Group LeftDT ({FL, ML, BL});
Motor_Group RightDT ({FR, MR, BR});
Motor_Group cata ({Cata1, Cata2});




