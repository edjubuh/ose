/**
 * @file vulcan/lift.c		
 * @brief Source file for lift functions.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "vulcan/Lift.h"

#include "sml/SmartMotorLibrary.h"
#include "sml/MasterSlavePIDController.h"
#include "sml/SingleThreadPIDController.h"

#include "vulcan/CortexDefinitions.h"

#define IME_RESET_THRESHOLD		100
#define POT_RESET_THRESHOLD		200

static PIDController Controller;
static TaskHandle LiftControllerTask;

// ---------------- LEFT  SIDE ---------------- //
/**
 * @brief Sets the speed of the left side of the lift
 *
 * @param value
 *		[-127,127] Desired PWM value of the left side of the lift
 *
 * @param immediate
 *		Determines if speed input change is immediate or ramped according to SML
 */
void LiftSetLeft(int value, bool immediate)
{
	if ((digitalRead(DIG_LIFT_BOTLIM) == LOW && value < 0) || (digitalRead(DIG_LIFT_TOPLIM) == LOW && value > 0))
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, 0, true);
		MotorSet(MOTOR_LIFT_REARLEFT, 0, true);
		MotorSet(MOTOR_LIFT_MIDDLELEFT, 0, true);
	}
	else if (value > 100)
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, 100, immediate);
		MotorSet(MOTOR_LIFT_REARLEFT, 100, immediate);
		MotorSet(MOTOR_LIFT_MIDDLELEFT, 100, immediate);
	}
	else
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, value, immediate);
		MotorSet(MOTOR_LIFT_REARLEFT, value, immediate);
		MotorSet(MOTOR_LIFT_MIDDLELEFT, value, immediate);
	}
}

/**
 * @brief Returns the calibrated value of the left potentiometer.
 *		  Calibrated by taking the average of the last twenty calls.
 *		  A zero point is taken whenever the the bottom limit switch is pressed.
 */
int LiftGetCalibratedPotentiometerLeft()
{
	static int zeroValue = 1435;
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = -(LiftGetRawPotentiometerLeft() - zeroValue);
	int sum = 0;
	for (int i = 0; i < 10; i++)
		sum += prevValues[i];

	int out = (int)(sum / 10.0);

	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
	{
		zeroValue = LiftGetRawPotentiometerLeft();
		out = 0;
	}

	return out;
}

/**
 * @brief Returns the raw value of the left potentiomter
 */
int LiftGetRawPotentiometerLeft()
{
	return -analogRead(ANA_POT_LIFT_LEFT);
}

static int liftComputeCorrectedSpeedLeft(int in)
{
	static int lastValue;
	static int lastTime;

	int out = in;

	if (in != 0)
	{
		int speed = (LiftGetCalibratedPotentiometerLeft() - lastValue) / ((millis() - lastTime) * 0.001);

		if (speed != 0 && motorGet(MOTOR_LIFT_FRONTLEFT) != 0)
			out = (in * in * 40) / (127 * speed);
		else out = in;
	}
	else out = 0;

	lastValue = LiftGetCalibratedPotentiometerLeft();
	lastTime = millis();

	return out;
}

// ---------------- RIGHT SIDE ---------------- //
/**
 * @brief Sets the speed of the right side of the lift
 *
 * @param value
 *		[-127,127] Desired PWM value of the right side of the lift
 *
 * @param immediate
 *		Determines if speed input change is immediate or ramped according to SML
 */
void LiftSetRight(int value, bool immediate)
{
	if ((digitalRead(DIG_LIFT_BOTLIM) == LOW && value < 0) || (digitalRead(DIG_LIFT_TOPLIM) == LOW && value > 0))
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT, 0, true);
		MotorSet(MOTOR_LIFT_REARRIGHT, 0, true);
		MotorSet(MOTOR_LIFT_MIDDLERIGHT, 0, true);
	}
	else if (value > 100)
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT, 100, immediate);
		MotorSet(MOTOR_LIFT_REARRIGHT, 100, immediate);
		MotorSet(MOTOR_LIFT_MIDDLERIGHT, 100, immediate);
	}
	else
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT, value, immediate);
		MotorSet(MOTOR_LIFT_REARRIGHT, value, immediate);
		MotorSet(MOTOR_LIFT_MIDDLERIGHT, value, immediate);
	}
}

/**
 * @brief Returns the calibrated value of the right potentiometer
 */
int LiftGetCalibratedPotentiometerRight()
{
	static int zeroValue = -210;
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawPotentiometerRight() - zeroValue;

	int sum = 0;
	for (int i = 0; i < 10; i++)
		sum += prevValues[i];

	int out = (int)(sum / 10.0);

	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
	{
		zeroValue = LiftGetRawPotentiometerRight();
		out = 0;
	}

	return out;
}

/**
 * @brief Returns the raw value of the right potentiomter
 */
int LiftGetRawPotentiometerRight()
{
	return analogRead(ANA_POT_LIFT_RIGHT);
}

static int liftComputeCorrectedSpeedRight(int in)
{
	static int lastValue;
	static int lastTime;

	int out;

	if (in != 0)
	{
		int speed = (LiftGetCalibratedPotentiometerRight() - lastValue) / ((millis() - lastTime) * 0.001);

		if (speed != 0 && motorGet(MOTOR_LIFT_FRONTRIGHT) != 0)
			out = (motorGet(MOTOR_LIFT_FRONTRIGHT) * motorGet(MOTOR_LIFT_FRONTRIGHT) * 40) / (127 * speed);
		else out = in;
	}
	else out = 0;

	lastValue = LiftGetCalibratedPotentiometerRight();
	lastTime = millis();

	return out;
}

// ---------------- MASTER (ALL) ---------------- //
/**
 * @brief Sets the lift to the desired speed using the MasterSlavePIDController for the lift
 *
 * @param value
 *			[-127,127] Speed of the lift
 *
 * @param immediate
 *			Determines if speed input change is immediate or ramped according to SML
 */
void LiftSet(int value, bool immediate)
{
	//MasterSlavePIDSetOutput(&Controller, value);
	if (value < -90) value = -90;
	LiftSetLeft(value, immediate);
	LiftSetRight(value, immediate);
}

/**
 * @brief Sets the lift goal height to the supplied parameter.
 *
 * @param value
 *			The new goal height of the lift.
 */
bool LiftSetHeight(int value)
{
	PIDControllerSetGoal(&Controller, value);
	return false;
}

bool LiftContinuous()
{
	return PIDControllerExecuteContinuous(&Controller);
}

/**
 * @brief Returns the difference between the potentiometers (right - left)
 *        Used in the equalizer controller in the MasterSlavePIDController for the lift
 */
int liftComputePotentiometerDifference()
{
	return LiftGetCalibratedPotentiometerRight() - LiftGetCalibratedPotentiometerLeft();
}

/**
 * @brief Initializes the lift motors and PID controllers
 */
void LiftInitialize()
{
	MotorConfigure(MOTOR_LIFT_FRONTLEFT, true, 0.25);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT, false, 0.25);
	MotorConfigure(MOTOR_LIFT_MIDDLELEFT, true, 0.25);
	MotorConfigure(MOTOR_LIFT_MIDDLERIGHT, false, 0.25);
	MotorConfigure(MOTOR_LIFT_REARLEFT, false, 0.25);
	MotorConfigure(MOTOR_LIFT_REARRIGHT, false, 0.25);

	/*MotorChangeRecalculateCommanded(MOTOR_LIFT_FRONTLEFT, &liftComputeCorrectedSpeedLeft);
	MotorChangeRecalculateCommanded(MOTOR_LIFT_FRONTRIGHT, &liftComputeCorrectedSpeedRight);
	MotorChangeRecalculateCommanded(MOTOR_LIFT_REARLEFT, &liftComputeCorrectedSpeedLeft);
	MotorChangeRecalculateCommanded(MOTOR_LIFT_REARRIGHT, &liftComputeCorrectedSpeedRight);*/
	
	unsigned long start = millis();
	while ((millis() - start) < 250)
	{ // Calibrate potentiometers
		LiftGetCalibratedPotentiometerRight();
		LiftGetCalibratedPotentiometerLeft();
		liftComputeCorrectedSpeedRight(0);
		liftComputeCorrectedSpeedRight(0);
		delay(5);
	}
	delay(50);
	/*
	//                                           Execute           Call							     Kp  Ki   Kd    MaI  MiI  Tol
	PIDController master = PIDControllerCreate(&LiftSetLeft, &LiftGetCalibratedPotentiometerLeft,	0.65, 0, 0, 300, -200, 5);
	PIDController slave = PIDControllerCreate(&LiftSetRight, &LiftGetCalibratedPotentiometerRight, 0.65, 0, 0, 300, -200, 5);
	PIDController equalizer = PIDControllerCreate(NULL, &liftComputePotentiometerDifference, 0, 0, 0, 50, -50, 5);

	Controller = CreateMasterSlavePIDController(master, slave, equalizer, false);

	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
	*/

	Controller = PIDControllerCreate(&LiftSet, &LiftGetCalibratedPotentiometerLeft, 0.19, 0.08, -0.3, 300, -100, 15);
}