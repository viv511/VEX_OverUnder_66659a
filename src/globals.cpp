#include "main.h"
#include "globals.h"

pros::Motor FL(16, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor ML(12, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor BL(4, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor FR(19, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor MR(7, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor BR(14, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor Cata1(9, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor Cata2(11, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);//?

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::IMU inertial(3);
pros::Rotation leftRot(17);
pros::Rotation rightRot(15);

//think these are ok
pros::ADIDigitalOut wings('A');
pros::ADIDigitalOut intake('B');

pros::Motor_Group LeftDT ({FL, ML, BL});
pros::Motor_Group RightDT ({FR, MR, BR});
pros::Motor_Group cata ({Cata1, Cata2});




