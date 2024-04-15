#include "main.h"

std::pair<float, float> joystickToPower() {
	float axisOne = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) / 127.0);
	float axisTwo = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)) / 127.0) * 0.88; 

	// Secret Sauce
	axisTwo *= 100;
	axisTwo = exp((fabs(axisTwo) - 100) / 50) * axisTwo;
	axisTwo /= 100;

	float mag = fmax(1.0, fmax(fabs(axisOne + axisTwo), fabs(axisOne - axisTwo)));


	//-1 <--  0 --> 1 scale to velocity (-600 <-- 0 --> 600 RPM)
	float leftPower = ((axisOne + axisTwo) / mag) * 600;
	float rightPower = ((axisOne - axisTwo) / mag) * 600;

	return std::make_pair(leftPower, rightPower);
}

void initialize()
{
	wings.set_value(false);
	backWing.set_value(false);

	pros::lcd::initialize();
	pros::Task trackRobot(tracking);

	LeftDT.set_brake_modes(MOTOR_BRAKE_HOLD);
	RightDT.set_brake_modes(MOTOR_BRAKE_HOLD);
}


void disabled() {
	wings.set_value(false);
	backWing.set_value(false);
}

void competition_initialize() {
	wings.set_value(false);
	backWing.set_value(false);
}

void autonomous() { 
	
}

void opcontrol() {
	LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

	// Toggle's
	bool elevationState = false;
	bool elevationLast = false;

	while(true) {
		// *---*---*---*---*---*---*--CONTROLLER AND DRIVE--*---*---*---*---*---*---*---*---*
		LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
		RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

		std::pair<float, float> powers = joystickToPower();

		// Assign power
		LeftDT.move_velocity(powers.first);
		RightDT.move_velocity(powers.second);

		//BACK WINGS
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			backWing.set_value(true);
		}
		else
		{
			backWing.set_value(false);
		}

		// WINGS
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			wings.set_value(true);
		}
		else
		{
			wings.set_value(false);
		}

		
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			intake.move_voltage(-12000);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			intake.move_voltage(12000);
		}
		else
		{
			intake.move_voltage(0);
		}
		
		pros::delay(10);
	}
}