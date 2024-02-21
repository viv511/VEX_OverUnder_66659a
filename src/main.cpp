#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	wings.set_value(false);
	backWing.set_value(false);
	//intakePiston.set_value(false);

	pros::lcd::initialize();
	pros::Task trackRobot(tracking);
	// pros::Task cataControl(cataCtrl);

	LeftDT.set_brake_modes(MOTOR_BRAKE_HOLD);
	RightDT.set_brake_modes(MOTOR_BRAKE_HOLD);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	//intakePiston.set_value(false);
	wings.set_value(false);
	backWing.set_value(false);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	//intakePiston.set_value(false);
	wings.set_value(false);
	backWing.set_value(false);
	// pros::lcd::print(0, "Competition Initialize");
	// pros::lcd::print(1, "Team 315Y");
	// pros::lcd::print(2, "VEX Robotics Competition");
	// pros::lcd::print(3, "Tower Takeover");
	// pros::lcd::print(4, "Autonomous Selector");
	// pros::lcd::print(5, "Left: Defensive Rush");
	// pros::lcd::print(6, "Right: Defensive Push");
	// pros::lcd::print(7, "Center: Skills");
	// pros::lcd::print(8, "Down: Disrupt");
	// pros::lcd::print(9, "Up: Offensive Sneak");
	// pros::lcd::print(10, "Down: Defensive Rush");
	// pros::lcd::print(11, "Right: Defensive Push");
	// pros::lcd::print(12, "Center: Skills");
	// pros::lcd::print(13, "Down: Disrupt");
	// pros::lcd::print(14, "Up: Offensive Sneak");
	// pros::lcd::print(15, "Down: Defensive Rush");
	// pros::lcd::print(16, "Right: Defensive Push");
	// pros::lcd::print(17, "Center: Skills");
	// pros::lcd::print(18, "Down: Disrupt");
	// pros::lcd::print(19, "Up: Offensive Sneak");
	// pros::lcd::print(20, "Down: Defensive Rush");
	// pros::lcd::print(21, "Right: Defensive Push");
	// pros::lcd::print(22, "Center: Skills");
	// pros::lcd::print(23, "Down: Disrupt");
	// pros::lcd::print(24, "Up: Offensive Sneak");
	// pros::lcd::print(25, "Down: Defensive Rush");
	// pros::lcd::print(26, "Right: Defensive Push");
	// pros::lcd::print(27, "Center: Skills");
	// pros::lcd::print(28, "Down: Disrupt");
	// pros::lcd::print(29, "Up: Offensive Sneak");
}

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
	
	defensiveRush();

	// intakePiston.set_value(1);	
	// defensivePush();
	// skillz2();
	// skillz3();
	// disrupt();
	//offensiveSneak();
	// defenseRoute();
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
	//intakePiston.set_value(true);

	LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	float axisOne;
	float axisTwo;
	float leftPower;
	float rightPower;

	// Toggle's
	bool elevationState = false;
	bool elevationLast = false;

	while (1) {
		LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
		RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

		// *---*---*---*---*---*---*--CONTROLLER AND DRIVE--*---*---*---*---*---*---*---*---*
		axisOne = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) / 127.0);
		axisTwo = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)) / 127.0) * 0.88; 
		// 0.88 is turn senstivity

		// Secret Sauce
		axisTwo *= 100;
		axisTwo = exp((fabs(axisTwo) - 100) / 50) * axisTwo;
		axisTwo /= 100;

		float mag = fmax(1.0, fmax(fabs(axisOne + axisTwo), fabs(axisOne - axisTwo)));

		//-1 <--  0 --> 1 scale to velocity (-600 <-- 0 --> 600 RPM)
		leftPower = ((axisOne + axisTwo) / mag) * 600;
		rightPower = ((axisOne - axisTwo) / mag) * 600;

		// Assign power
		LeftDT.move_velocity(leftPower);
		RightDT.move_velocity(rightPower);

		// CATA
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))
		{
			cata.move_voltage(12000);
		}
		else
		{
			cata.move_voltage(0);
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

		// INTAKE
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
		{
			intake.move_voltage(12000);
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			intake.move_voltage(-12000);
		}
		else
		{
			intake.move_voltage(0);
		}

		
		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			backWing.set_value(1);
		}
		else {
			backWing.set_value(0);
		}

		// if ((controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) && !elevationLast)
		// {
		// 	elevationState = !elevationState;
		// 	elevationLast = true;
		// }
		// else if (!((controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))))
		// {
		// 	elevationLast = false;
		// }

		// if (elevationState)
		// {
		// 	backWing.set_value(true);
		// }
		// else
		// {
		// 	backWing.set_value(false);
		// }

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		}
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		}
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
		}
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
		}
		
		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			
		}
	
		// controller.print(0,1,"%f",inertial.get_rotation());
		pros::delay(10);
	}
}
