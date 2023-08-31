#include "main.h"

pros::Motor FL(7, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor ML(11, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor BL(16, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor FR(5, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor MR(8, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor BR(10, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor_Group LeftDT ({FL, ML, BL});
pros::Motor_Group RightDT ({FR, MR, BR});