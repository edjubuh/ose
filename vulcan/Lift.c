/**
 * @file vulcan/lift.c		
 * @brief Source file for lift functions.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include <string.h>

#include "main.h"
#include "vulcan/Lift.h"

#include "lcd/LCDFunctions.h"
#include "sml/SmartMotorLibrary.h"
#include "sml/MasterSlavePIDController.h"
#include "sml/SingleThreadPIDController.h"

#include "vulcan/CortexDefinitions.h"

#define IME_RESET_THRESHOLD		100
#define QUAD_ENC_MAX_DIF		10
#define MAX_DOWN_PWM			-100
#define LIFT_SKEW_RATE			10

static Encoder rightEncoder, leftEncoder;
// ---------------- LEFT  SIDE ---------------- //
/**
 * @brief Sets the speed of the left side of the lift
 *
 * @param value
 *		[-100,127] Desired PWM value of the left side of the lift
 *
 * @param immediate
 *		Determines if speed input change is immediate or ramped according to SML
 */
void LiftSetLeft(int value, bool immediate)
{
	if ((value < 0 && digitalRead(DIG_LIFT_BOTLIM) == LOW) || (value > 0 && digitalRead(DIG_LIFT_TOPLIM_LEFT) == LOW))
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, 0, true);
		MotorSet(MOTOR_LIFT_REARLEFT, 0, true);
		MotorSet(MOTOR_LIFT_MIDDLELEFT, 0, true);
	}
	else if (value < MAX_DOWN_PWM)
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT, MAX_DOWN_PWM, immediate);
		MotorSet(MOTOR_LIFT_REARLEFT, MAX_DOWN_PWM, immediate);
		MotorSet(MOTOR_LIFT_MIDDLELEFT, MAX_DOWN_PWM, immediate);
	}
	else
	{
		MotorSet(MOTOR_LIFT_FRONTLEFT,  value, immediate);
		MotorSet(MOTOR_LIFT_REARLEFT,   value, immediate);
		MotorSet(MOTOR_LIFT_MIDDLELEFT, value, immediate);
	}
}

/**
* @brief Returns the calibrated value of the left lift IME
*/
int LiftGetCalibIMELeft()
{
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawIMELeft();

	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
	{
		imeReset(I2C_MOTOR_LIFT_LEFT);
		memset(prevValues, 0, sizeof(prevValues));
		return 0;
	}

	int sum = 0;
	for (int i = 0; i < 10; i++)
		sum += prevValues[i];
	return (int)(sum / 10.0);
}

/**
* @brief Returns the raw value of the left lift IME (corrected for inversion)
*/
int LiftGetRawIMELeft()
{
	int val;
	imeGet(I2C_MOTOR_LIFT_LEFT, &val);
	return val;
}

/**
 * @brief Returns the value of the left lift quadrature encoder (located at the top of the lift for stabilization)
 */
int LiftGetQuadEncLeft()
{
	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
		encoderReset(leftEncoder);

	return encoderGet(leftEncoder);
}

int LiftGetCalibPotLeft()
{
	static int zeroValue = 0;
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawPotLeft() - zeroValue;

	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
	{
		zeroValue = LiftGetRawPotLeft();
		memset(prevValues, 0, sizeof(prevValues));
		return 0;
	}

	int sum = 0;
	for (int i = 0; i < 10; i++)
		sum += prevValues[i];
	return (int)(sum / 10.0);
}

int LiftGetRawPotLeft()
{
	return analogRead(ANA_POT_LIFT_LEFT)/10;
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
	if ((value < 0 && digitalRead(DIG_LIFT_BOTLIM) == LOW) || (value > 0 && digitalRead(DIG_LIFT_TOPLIM_LEFT) == LOW))
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT,  0, true);
		MotorSet(MOTOR_LIFT_REARRIGHT,   0, true);
		MotorSet(MOTOR_LIFT_MIDDLERIGHT, 0, true);
	}
	else if (value < MAX_DOWN_PWM)
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT, MAX_DOWN_PWM, immediate);
		MotorSet(MOTOR_LIFT_REARRIGHT, MAX_DOWN_PWM, immediate);
		MotorSet(MOTOR_LIFT_MIDDLERIGHT, MAX_DOWN_PWM, immediate);
	}
	else
	{
		MotorSet(MOTOR_LIFT_FRONTRIGHT,  value, immediate);
		MotorSet(MOTOR_LIFT_REARRIGHT,   value, immediate);
		MotorSet(MOTOR_LIFT_MIDDLERIGHT, value, immediate);
	}
}

/**
 * @brief Returns the calibrated value of the right lift IME
 */
int LiftGetCalibIMERight()
{
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawIMERight();

	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
	{
		imeReset(I2C_MOTOR_LIFT_RIGHT);
		memset(prevValues, 0, sizeof(prevValues));
		return 0;
	}

	int sum = 0;
	for (int i = 0; i < 10; i++)
		sum += prevValues[i];
	return (int)(sum / 10.0);
}

/** 
 * @brief Returns the raw value of the right lift IME (corrected for inversion)
 */
int LiftGetRawIMERight()
{
	int val;
	imeGet(I2C_MOTOR_LIFT_RIGHT, &val);
	return -val;
}

/**
* @brief Returns the value of the left right quadrature encoder (located at the top of the lift for stabilization)
*/
int LiftGetQuadEncRight()
{
	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
		encoderReset(rightEncoder);

	return encoderGet(rightEncoder);
}

int LiftGetCalibPotRight()
{
	static int zeroValue = 0;
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawPotRight() - zeroValue;

	if (digitalRead(DIG_LIFT_BOTLIM) == LOW)
	{
		zeroValue = LiftGetRawPotRight();
		memset(prevValues, 0, sizeof(prevValues));
		return 0;
	}

	int sum = 0;
	for (int i = 0; i < 10; i++)
		sum += prevValues[i];
	return (int)(sum / 10.0);
}

int LiftGetRawPotRight()
{
	return analogRead(ANA_POT_LIFT_RIGHT)/10;
}


// ---------------- MASTER (ALL) ---------------- //
static MasterSlavePIDController Controller;
static TaskHandle LiftControllerTask;
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
	// For enabled MasterSlavePIDController
	MasterSlavePIDSetOutput(&Controller, value);

	// For disabled MasterSlavePIDControlller
	//LiftSetLeft(value, immediate);
	//LiftSetRight(value, immediate);
}

/**
 * @brief Sets the lift goal height to the supplied parameter.
 *
 * @param value
 *			The new goal height of the lift.
 */
bool LiftSetHeight(int value)
{
	if (value == 0)
		MasterSlavePIDSetOutput(&Controller, -100);
	else MasterSlavePIDSetGoal(&Controller, value);
	return false;
}

/**
 * @brief Goes to intended height to completion
 *
 * @param value
 *			The new goal height of the lift
 */
void LiftGoToHeightCompletion(int value)
{
	if (value == 0)
	{
		LiftSet(-100, false);
		while (digitalRead(DIG_LIFT_BOTLIM) == HIGH) delay(10);
		LiftSet(0, false);;
	}
	else
	{
		MasterSlavePIDSetGoal(&Controller, value);
		while (!MasterSlavePIDOnTarget(&Controller)) delay(100);
	}
}

/**
 * @brief Returns true if the PID controller is on target
 */
bool LiftGoToHeightContinuous(int value)
{
	MasterSlavePIDSetGoal(&Controller, value);
	return MasterSlavePIDOnTarget(&Controller);
}

/**
 * @brief Returns the difference between the IMES (right - left)
 *		  Used in the equailizer controller in the MasterSlavePIDController for the lift
 */
static int liftComputeIMEDiff()
{
	return LiftGetCalibIMERight() - LiftGetCalibIMELeft();
}

/**
 * @brief Returns the difference between the quadrature encoders (right - left)
 *		  Used in the equalizer controller in the MasterSlavePIDController for the lift
 */
static int liftComputeQuadEncDiff()
{
	// Only compute difference if lift has hit bottom at soem point
	static bool encCorrect = false;
	if (digitalRead(DIG_LIFT_BOTLIM) == LOW) encCorrect = true;
	if (!encCorrect) return 0;

	// If the difference between the two is greater than the maximum difference
	//		pretend that we're on target because something is going massively wrong (i.e. hitting guidance bars)
	if (abs(LiftGetQuadEncRight() - LiftGetQuadEncLeft()) > QUAD_ENC_MAX_DIF) return 0;

	// If we're both on top, don't fix
	if ((digitalRead(DIG_LIFT_TOPLIM_LEFT) == LOW || digitalRead(DIG_LIFT_TOPLIM_RIGHT) == LOW) || digitalRead(DIG_LIFT_BOTLIM) == LOW) return 0;;
	return LiftGetQuadEncRight() - LiftGetQuadEncLeft();
}

/**
 * @brief Returns the difference between the potentiometers (right - left)
 *		  Used in the equalizer controller in the MasterSlavePIDController for the lift
 */
static int liftComputePotDiff()
{
	return 0;
}

/**
 * @brief Initializes the lift motors and PID controllers
 */
void LiftInitialize()
{
	MotorConfigure(MOTOR_LIFT_FRONTLEFT,	true, LIFT_SKEW_RATE);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT,	false, LIFT_SKEW_RATE);
	MotorConfigure(MOTOR_LIFT_MIDDLELEFT,	true, LIFT_SKEW_RATE);
	MotorConfigure(MOTOR_LIFT_MIDDLERIGHT,	false, LIFT_SKEW_RATE);
	MotorConfigure(MOTOR_LIFT_REARLEFT,		false, LIFT_SKEW_RATE);
	MotorConfigure(MOTOR_LIFT_REARRIGHT,	false, LIFT_SKEW_RATE);
		
	leftEncoder = encoderInit(DIG_LIFT_ENC_LEFT_TOP, DIG_LIFT_ENC_LEFT_BOT, false);
	rightEncoder = encoderInit(DIG_LIFT_ENC_RIGHT_TOP, DIG_LIFT_ENC_RIGHT_BOT, true);
	
	//                                           Execute           Call			    Kp    Ki   Kd MaI  MiI  Tol
	PIDController master = PIDControllerCreate(&LiftSetLeft, &LiftGetQuadEncLeft,  2.25, 0.4, 0, 100, -75, 4);
	PIDController slave = PIDControllerCreate(&LiftSetRight, &LiftGetQuadEncRight, 2.25, 0.4, 0, 100, -75, 4);
	PIDController equalizer = PIDControllerCreate(NULL, &liftComputeQuadEncDiff,   0.95, 0.4, 0, 100, -75, 3);

	Controller = CreateMasterSlavePIDController(master, slave, equalizer, 105, -100, false);

	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
}