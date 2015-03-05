/**
 * @file vulcan/lift.c	
 * @author Elliot Berman and Rob Shrote
 * @brief Source file for lift functions.
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
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
#define LIFT_SKEW_RATE			1.75
#define QUAD_ENC_MIN_THRESH		8

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
	if ((value < 0 && digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW) || (value > 0 && digitalRead(DIG_LIFT_TOPLIM_LEFT) == LOW))
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

	if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW)
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
	if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW)
		encoderReset(leftEncoder);

	return encoderGet(leftEncoder);
}

/**
* @brief Returns the calibrated potentiometer left value.
*		  The value is calibrated by taking the average of relative readings to the ground over the previous 20 calls
* @deprecated Potentiometers not currently installed.
*/
int LiftGetCalibPotLeft()
{
	static int zeroValue = 0;
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawPotLeft() - zeroValue;

	if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW)
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

/**
* @brief Returns the raw potentioemter value (chopped off single digit for reduced noise)
* @deprecated Potentiometers not currently installed.
*/
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
	if ((value < 0 && digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW) || (value > 0 && digitalRead(DIG_LIFT_TOPLIM_LEFT) == LOW))
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

	if (digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW)
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
	if (digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW)
		encoderReset(rightEncoder);

	return encoderGet(rightEncoder);
}

/**
 * @brief Returns the calibrated potentiometer right value. 
 *		  The value is calibrated by taking the average of relative readings to the ground over the previous 20 calls
 * @deprecated Potentiometers not currently installed.
 */
int LiftGetCalibPotRight()
{
	static int zeroValue = 0;
	static int prevValues[10];
	for (int i = 0; i < 9; i++)
		prevValues[i] = prevValues[i+1];
	prevValues[9] = LiftGetRawPotRight() - zeroValue;

	if (digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW)
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

/**
 * @brief Returns the raw potentioemter value (chopped off single digit for reduced noise)
 * @deprecated Potentiometers not currently installed.
 */
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
		while (digitalRead(DIG_LIFT_BOTLIM_LEFT) == HIGH) delay(10);
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
	// Only compute difference if lift has hit bottom at some point
	static bool encCorrect = false;
	if (!encCorrect)
	{
		if (digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW && digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW) encCorrect = true;
		else return 0;
	}

	// If the difference between the two is greater than the maximum difference
	//		pretend that we're on target because something is going massively wrong (i.e. hitting guidance bars)
	if (abs(LiftGetQuadEncRight() - LiftGetQuadEncLeft()) > QUAD_ENC_MAX_DIF) return 0;

	// If any limit switch is pressed, don't correct heights
	if (digitalRead(DIG_LIFT_TOPLIM_LEFT) == LOW || digitalRead(DIG_LIFT_TOPLIM_RIGHT) == LOW || 
		digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW || digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW) 
		return 0;

	// Don't correct if too low to get reliable data from encoders
	if (LiftGetQuadEncRight() < QUAD_ENC_MIN_THRESH || LiftGetQuadEncLeft() < QUAD_ENC_MIN_THRESH)
		return 0;

	return LiftGetQuadEncRight() - LiftGetQuadEncLeft();
}

/**
 * @brief Returns the difference between the potentiometers (right - left)
 *		  Used in the equalizer controller in the MasterSlavePIDController for the lift
 * @deprecated Potentiometers not installed.
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
	
	//                                           Execute           Call			    Kp    Ki   Kd   MaI  MiI  Tol
	PIDController master = PIDControllerCreate(&LiftSetLeft, &LiftGetQuadEncLeft,  3.16, 0.18, 0.15, 150, -75, 3);
	PIDController slave = PIDControllerCreate(&LiftSetRight, &LiftGetQuadEncRight, 3.16, 0.18, 0.15, 150, -75, 3);
	PIDController equalizer = PIDControllerCreate(NULL, &liftComputeQuadEncDiff,   0.90, 0.45, 0.01, 100, -75, 3);

	Controller = CreateMasterSlavePIDController(master, slave, equalizer, 105, -100, false);

	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
}