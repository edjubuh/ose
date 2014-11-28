/************************************************************************/
/* @file lift.c															*/
/* @brief Source file for lift functions.								*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "dios/Lift.h"
#include "dios/CortexDefinitions.h"
#include "sml/SmartMotorLibrary.h"
#include "sml/MasterSlavePIDController.h"
#include "sml/SingleThreadPIDController.h"
#include <math.h>

#define IME_RESET_THRESHOLD		100
#define POT_RESET_THRESHOLD		200

static MasterSlavePIDController Controller;
static TaskHandle LiftControllerTask;
Gyro gyro;

// ---------------- LEFT  SIDE ---------------- //
/**
* Sets the speed of the left side of the lift
* @param value
*		[-127,127] Desired PWM value of the left side of the lift
* @param immediate
*		Determines if speed input change is immediate or ramped according to SML
*/
void LiftSetLeft(int value, bool immediate)
{
	if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW && value < 0)
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, 0, immediate);
		MotorSet(MOTOR_LIFT_REARLEFT, 0, immediate);
		MotorSet(MOTOR_LIFT_THLEFT, 0, immediate);
	}
	else
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, value, immediate);
		MotorSet(MOTOR_LIFT_REARLEFT, value, immediate);
		MotorSet(MOTOR_LIFT_THLEFT, value, immediate);
	}
}

/**
* Returns the calibrated value of the left potentiometer (in-house calibration)
*/
int LiftGetCalibratedPotentiometerLeft()
{
	static int zeroValue = 1435;
	static int prevValues[20];
	for (int i = 0; i < 19; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[19] = LiftGetRawPotentiometerLeft() - zeroValue;
	int sum = 0;
	for (int i = 0; i < 20; i++)
		sum += prevValues[i];

	int out = (int)(sum / 20.0);

	if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW)
	{
		zeroValue = LiftGetRawPotentiometerLeft();
		out = 0;
	}

	return out;
}

/**
* Returns the raw value of the left potentiomter
*/
int LiftGetRawPotentiometerLeft()
{
	return analogRead(ANA_POT_LIFT_LEFT);
}

/**
* Returns the raw left IME.
* Retired. Use the potentiometer to get the current height of the lift
*/
int LiftGetEncoderLeft()
{
	int value;
	imeGet(I2C_MOTOR_LIFT_LEFT, &value);
	if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW)
	{
		imeReset(I2C_MOTOR_LIFT_LEFT);
		value = 0;
	}
	return value;
}

// ---------------- RIGHT SIDE ---------------- //
/**
 * Sets the speed of the right side of the lift
 * @param value
 *		[-127,127] Desired PWM value of the right side of the lift
 * @param immediate
 *		Determines if speed input change is immediate or ramped according to SML
 */
void LiftSetRight(int value, bool immediate)
{
	if (digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW && value < 0)
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT, 0, immediate);
		MotorSet(MOTOR_LIFT_REARRIGHT, 0, immediate);
		MotorSet(MOTOR_LIFT_THRIGHT, 0, immediate);
	}
	else
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT, value, immediate);
		MotorSet(MOTOR_LIFT_REARRIGHT, value, immediate);
		MotorSet(MOTOR_LIFT_THRIGHT, value, immediate);
	}
}

/**
* Returns the calibrated value of the right potentiometer
*/
int LiftGetCalibratedPotentiometerRight()
{
	static int zeroValue = -210;
	static int prevValues[20];
	for (int i = 0; i < 19; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[19] = LiftGetRawPotentiometerRight() - zeroValue;

	int sum = 0;
	for (int i = 0; i < 20; i++)
		sum += prevValues[i];

	int out = (int)(sum / 20.0);

	if (digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW)
	{
		zeroValue = LiftGetRawPotentiometerRight();
		out = 0;
	}

	return out;
}

/**
 * Returns the raw value of the right potentiomter
 */
int LiftGetRawPotentiometerRight()
{
	return -analogRead(ANA_POT_LIFT_RIGHT);
}

/**
* Returns the raw right IME.
* Retired. Use the potentiometer to get the current height of the lift
*/
int LiftGetEncoderRight()
{
	int value;
	imeGet(I2C_MOTOR_LIFT_RIGHT, &value);
	value = -value;
	if (value < IME_RESET_THRESHOLD && digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW)
	{
		imeReset(I2C_MOTOR_LIFT_RIGHT);
		value = 0;
	}
	return value;
}

// ---------------- MASTER (ALL) ---------------- //
/**
* Sets the lift to the desired speed using the MasterSlavePIDController for the lift
* @param value
*			[-127,127] Speed of the lift
* @param immediate
*			Determines if speed input change is immediate or ramped according to SML
*/
void LiftSet(int value)
{
	MasterSlavePIDIncreaseGoal(&Controller, value);
	//LiftSetLeft(value, false);
	//LiftSetRight(value, false);
}


int liftComputePotentiometerDifference()
{
	return LiftGetCalibratedPotentiometerRight() - LiftGetCalibratedPotentiometerLeft();
}

/**
* Initializes the lift motors and PID controllers
*/
void LiftInitialize()
{
	MotorConfigure(MOTOR_LIFT_FRONTLEFT, true, 1);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT, true, 1);
	MotorConfigure(MOTOR_LIFT_REARLEFT, true, 1);
	MotorConfigure(MOTOR_LIFT_REARRIGHT, true, 1);
	MotorConfigure(MOTOR_LIFT_THLEFT, true, 1);
	MotorConfigure(MOTOR_LIFT_THRIGHT, true, 1);
	
	unsigned long start = millis();
	while ((millis() - start) < 500)
	{ // Calibrate potentiometers at ground level
		LiftGetCalibratedPotentiometerRight();
		LiftGetCalibratedPotentiometerLeft();
		delay(5);
	}
	delay(100);

	PIDController master = PIDControllerCreate(&LiftSetLeft, &LiftGetCalibratedPotentiometerLeft, 0.4, 0.02, -0.03, 100, -100, 5);
	PIDController slave = PIDControllerCreate(&LiftSetRight, &LiftGetCalibratedPotentiometerRight, 0.4, 0.02, -0.03, 100, -100, 5);
	PIDController equalizer = PIDControllerCreate(NULL, &liftComputePotentiometerDifference, 0, 0, 0, 50, -50, 5);
	
	Controller = CreateMasterSlavePIDController(master, slave, equalizer, false);
	
	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
}