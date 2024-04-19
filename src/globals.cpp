#include "main.h"
#include "globals.h"

Motor FL(17, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); 
Motor ML(16, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); 
Motor BL(18, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); 
Motor FR(7, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); 
Motor MR(8, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); 
Motor BR(10, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES); 

Motor intakeLeft(15, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
Motor intakeRight(6, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES); 

Controller controller(E_CONTROLLER_MASTER);

IMU inertial(12); 
Rotation leftRot(9); 
Rotation rightRot(19);

ADIDigitalOut wings('H');
ADIDigitalOut backWing('C');
ADIDigitalOut hang('B');

Motor_Group LeftDT ({FL,ML,BL});
Motor_Group RightDT ({FR,MR,BR});
Motor_Group intake({intakeLeft, intakeRight});




