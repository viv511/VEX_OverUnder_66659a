#include "main.h"



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::Task trackRobot(tracking);
	wings.set_value(false);
	intake.set_value(false);
	// pros::Task cataControl(cataCtrl);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	float axisOne;
	float axisTwo;
	float leftPower;
	float rightPower;

	//For different driving formats: Split Arcade (s), Arcade (a), and Tank (t)
	char driveStyle = 's';

	//For joystick exponential curving
	bool expoDrive = true;

	//Wings
	bool wingState = false;
	bool wingLast = false;

	//Intake
	bool intakeState = false;
	bool intakeLast = false;

	while(1) {
		// *---*---*---*---*---*---*--CONTROLLER AND DRIVE--*---*---*---*---*---*---*---*---*
		if((driveStyle == 's') || (driveStyle == 'a')) {
			//Bind from -1 <-- 0 --> 1
			axisOne = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) / 127.0);
			axisTwo = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)) / 127.0);
			if(driveStyle == 'a') {
				axisTwo = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)) / 127.0);
			}

			//Find scaled bounded maximum for drivetrain %'s
			float mag = fmax(1.0, fmax(fabs(axisOne + axisTwo), fabs(axisOne - axisTwo)));

			//-1.0 <--  0.0 --> 1.0 scale to velocity (-600 <-- 0 --> 600 RPM)
			leftPower = ((axisOne + axisTwo) / mag) * 600;
			rightPower = ((axisOne - axisTwo) / mag) * 600;
		}
		else if(driveStyle == 't') {
			//Bind from -100 <-- 0 --> 100
			axisOne = 100 * ((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) / 127.0);
			axisTwo = 100 * ((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) / 127.0);

			if(expoDrive) {
				axisOne = exp((fabs(axisOne)-100)/50)*axisOne;
				axisTwo = exp((fabs(axisTwo)-100)/25)*axisTwo;
			}
			
			leftPower = axisOne*6;
			rightPower = axisTwo*6;
		}
		else {
			leftPower = 0;
			rightPower = 0;
		}

		//Assign power
		LeftDT.move_velocity(leftPower);
		RightDT.move_velocity(rightPower);

		//CATA
		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			cata.move_voltage(9000);
		}
		else {
			cata.move_voltage(0);
		}

		//WINGS
		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			wings.set_value(true);
		}
		else {
			wings.set_value(false);
		}

		//INTAKE
		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			intake.set_value(true);
		}
		else {
			intake.set_value(false);
		}

		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			turn(90);
		}
	

		
		pros::delay(10);
	}
}
