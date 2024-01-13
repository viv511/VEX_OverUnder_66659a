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
	// blocker.set_value(false);
	elev.set_value(false);

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
void autonomous()
{

	skillz2();
	// offensiveSneak();
	// offensiveRush();
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
void opcontrol()
{
	LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	float axisOne;
	float axisTwo;
	float leftPower;
	float rightPower;

	// Toggle's
	bool blockerState = false;
	bool blockerLast = false;

	bool elevationState = false;
	bool elevationLast = false;

	while (1)
	{
		LeftDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
		RightDT.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

		// *---*---*---*---*---*---*--CONTROLLER AND DRIVE--*---*---*---*---*---*---*---*---*
		axisOne = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) / 127.0);
		axisTwo = ((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)) / 127.0);

		// Secret Sauce
		axisTwo *= 100;
		axisTwo = exp((fabs(axisTwo) - 100) / 100) * axisTwo;
		axisTwo /= 100;

		float mag = fmax(1.0, fmax(fabs(axisOne + axisTwo), fabs(axisOne - axisTwo)));

		//-1 <--  0 --> 1 scale to velocity (-600 <-- 0 --> 600 RPM)
		leftPower = ((axisOne + axisTwo) / mag) * 600;
		rightPower = ((axisOne - axisTwo) / mag) * 600;

		// Assign power
		LeftDT.move_velocity(leftPower);
		RightDT.move_velocity(rightPower);

		// CATA
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			
			cata.move_voltage(12000 * 0.77);
		}
		else
		{
			cata.move_voltage(0);
		}

		// WINGS
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
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

		// if ((controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) && !blockerLast)
		// {
		// 	blockerState = !blockerState;
		// 	blockerLast = true;
		// }
		// else if (!((controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))))
		// {
		// 	blockerLast = false;
		// }

		// if (blockerState)
		// {
		// 	blocker.set_value(true);
		// }
		// else
		// {
		// 	blocker.set_value(false);
		// }

		if ((controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) && !elevationLast)
		{
			elevationState = !elevationState;
			elevationLast = true;
		}
		else if (!((controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))))
		{
			elevationLast = false;
		}

		if (elevationState)
		{
			elev.set_value(true);
		}
		else
		{
			elev.set_value(false);
		}

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
		{
			intake.move_velocity(12000);
			wings.set_value(1);
			pros::delay(200);
			wings.set_value(0);
			pivot(135);
			intake.move_velocity(0);
			driveDist(-34);
			pivot(75);
			LeftDT.move_voltage(-12000*0.5);
			RightDT.move_voltage(-12000*0.5);
			delay(300);
			stopMotors();
		}
	
		if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
			// setStart(26.22, -38.55);
			// followRoute("curve.txt", true);
			// turn(-45);
			// swervePoint(true, 0, 0, 90, true, 90);
			// RightDT.move_voltage(12000);
			// LeftDT.move_voltage(5000);
			// delay(550);
			// stopMotors();
			// driveDist(30);
			pivot(135);
			delay(500);
			pivot(75);
			//moveToPoint(Waypoint(20, 20), false);
		}

		pros::delay(10);
	}
}
