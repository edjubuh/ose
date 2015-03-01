/**
 * @file vulcan/Chassis.c
 * @author Elliot Berman and Robert Shrote
 * @brief Source file for Chassis functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include <math.h>

#include "main.h"
#include "vulcan/Chassis.h"

#include "sml/SmartMotorLibrary.h"
#include "sml/SingleThreadPIDController.h"
#include "lcd/LCDFunctions.h"
#include "vulcan/CortexDefinitions.h"

#define CHASSIS_SKEW_PROFILE	2.0

// ---------------- LEFT  SIDE ---------------- //
static PIDController leftController;
/**
 * @brief Sets the speed of the left motors on the chassis as specified by the parameters
 *
 * @param speed
 *		  [-127,127] The speed of the left side of the chassis
 * @param immediate
 *		  Determins if speed input change is immediate or ramped according to SML
 */
void ChassisSetLeft(int speed, bool immediate)
{
	if (abs(speed) > 127)
		speed = signbit(speed) ? -127 : 127;

	MotorSet(MOTOR_CHASSIS_FRONTLEFT, speed , immediate);
	MotorSet(MOTOR_CHASSIS_REARLEFT, speed, immediate);
}

/**
 * @brief Returns the value of the left chassis IME
 */
int ChassisGetIMELeft()
{
	int val; 
	imeGet(I2C_MOTOR_CHASSIS_LEFT, &val);
	return val;
}

/**
 * @brief Returns the value of the left side line follower
 */
int ChassisGetIRLeft()
{
	return analogRead(ANA_IR_LEFT);
}

/**
 * @brief Returns true if a line is detected, false otherwise on the left IR sensor
 *
 * @param tile
 *			The tile color the line will be on (different thresholds for different tiles)
 *
 * @returns Booleanr representing whether or not a line is detected
 */
bool ChassisHasIRLineLeft(kTiles tile)
{
	switch (tile)
	{
		case Red:
			return ChassisGetIRLeft() < CHASSIS_IR_LEFT_RED_THRESH;
		case Blue:
			return ChassisGetIRLeft() < CHASSIS_IR_LEFT_BLUE_THRESH;
		default:
			return ChassisGetIRLeft() < CHASSIS_IR_LEFT_GREY_THRESH;
	}
}

// ---------------- RIGHT  SIDE ---------------- //
static PIDController rightController;
/**
* @brief Sets the speed of the right motors on the chassis as specified by the parameters
*
* @param speed
*		  [-127,127] The speed of the right side of the chassis
* @param immediate
*		  Determins if speed input change is immediate or ramped according to SML
*/
void ChassisSetRight(int speed, bool immediate)
{
	if (abs(speed) > 127)
		speed = signbit(speed) ? -127 : 127;

	MotorSet(MOTOR_CHASSIS_FRONTRIGHT, speed, immediate);
	MotorSet(MOTOR_CHASSIS_REARRIGHT, speed, immediate);
}

/**
* @brief Returns the value of the right chassis IME
*/
int ChassisGetIMERight()
{
	int val;
	imeGet(I2C_MOTOR_CHASSIS_RIGHT, &val);
	return -val;
}

/**
 * @brief Returns the value of the right side line follower
 */
int ChassisGetIRRight()
{
	return analogRead(ANA_IR_RIGHT);
}

/**
 * @brief Returns true if a line is detected, false otherwise on the right IR sensor
 *
 * @param tile
 *			The tile color the line will be on (different thresholds for different tiles)
 *
 * @returns Booleanr representing whether or not a line is detected
 */
bool ChassisHasIRLineRight(kTiles tile)
{
	switch (tile)
	{
		case Red:
			return ChassisGetIRRight() < CHASSIS_IR_RIGHT_RED_THRESH;
		case Blue:
			return ChassisGetIRRight() < CHASSIS_IR_RIGHT_BLUE_THRESH;
		default:
			return ChassisGetIRRight() < CHASSIS_IR_RIGHT_GREY_THRESH;
	}
}

// ---------------- MASTER (ALL) ---------------- //
/**
 * @brief Sets the left and right motors of the chassis as specified by the parameters.
 *        Speeds of left and right will be scaled down relative to each other if either
 *        is greater than 127.
 *
 * @param left
 *        [-127,127] The speed of the left side of the chassis.
 * @param right
 *        [-127,127] The speed of the right side of the chassis.
 * @param immediate
 *        Determines if speed input change is immediate or ramped according to SML
 */
void ChassisSet(int left, int right, bool immediate)
{
	//statements below catch speeds out of range
	//catches left
	if (abs(left) > 127)
		left = signbit(left) ? -127 : 127;
	//catches right
	if (abs(right) > 127)
		right = signbit(right) ? -127 : 127;

	//set left and right wheel speeds according to parameters above.
	MotorSet(MOTOR_CHASSIS_FRONTLEFT, left, immediate);
	MotorSet(MOTOR_CHASSIS_FRONTRIGHT, right, immediate);
	MotorSet(MOTOR_CHASSIS_REARLEFT,   left,  immediate);
	MotorSet(MOTOR_CHASSIS_REARRIGHT,  right, immediate);
}

/**
 * @brief Sets the chassis to go at heading with speed and rotation. See additional
 *        mecanum conversion description in notebook.
 *
 * @param heading
 *        Direction of the chassis to go in radians (0 rad is forward, PI/2 is strafe
 *        left)
 * @param speed
 *        [-127,127] Speed of the chassis. At least one motor will go this speed, all
 *        others scaled appropriately to get kinematics
 * @param rotation
 *        [-127,127] Rotation of the chassis where 127 is clockwise (right) rotation.
 *        This is a final additive, so can overpower heading calculation
 * @param immediate
 *        Determines if speed input change is immediate or ramped according to SML
 */
void ChassisSetMecanum(double heading, int speed, int rotation, bool immediate)
{
	if (abs(speed) > 127)
		speed = signbit(speed) ? -127 : 127;
	if (abs(rotation) > 127)
		rotation = signbit(rotation) ? -127 : 127;

	int rightDiag = speed * cos(heading + (M_PI/4));
	int leftDiag = speed * cos(heading - (M_PI/4));

	int frontRight = rightDiag + rotation,
		rearRight = leftDiag + rotation,
		frontLeft = leftDiag - rotation,
		rearLeft = rightDiag - rotation;

	int max = 127;
	if (abs(frontRight) > max)
		max = abs(frontRight);
	if (abs(rearRight) > max)
		max = abs(rearRight);
	if (abs(frontLeft) > max)
		max = abs(frontLeft);
	if (abs(rearLeft) > max)
		max = abs(rearLeft);

	double speedScale = 127.00 / max;

	MotorSet(MOTOR_CHASSIS_FRONTRIGHT, (int)(frontRight * speedScale), immediate);
	MotorSet(MOTOR_CHASSIS_REARRIGHT,  (int)(rearRight * speedScale),  immediate);
	MotorSet(MOTOR_CHASSIS_FRONTLEFT,  (int)(frontLeft * speedScale),  immediate);
	MotorSet(MOTOR_CHASSIS_REARLEFT,   (int)(rearLeft * speedScale),   immediate);

}

/**
 * @brief Runs through one iteration of the PID Controllers, with the goal values being the ones in the parameters
 *
 * @param left
 *		  The left side goal value
 * @param right
 *		  The right side goal value
 *
 * @returns Returns true if BOTH sides are on target
 */
bool ChassisGoToGoalContinuous(int left, int right)
{
	PIDControllerSetGoal(&leftController, left);
	PIDControllerSetGoal(&rightController, right);

	// Use 1 '&' instead of 2 to force execution of both sides (both right and left side will run) because short-circuiting
	return PIDControllerExecuteContinuous(&leftController) & PIDControllerExecuteContinuous(&rightController);
}

/**
 * @brief Runs to completion of the PID Controllers, with the goal values being the ones in the parameters
 *
 * @param left
 *		  The left side goal value
 * @param right
 *		  The right side goal value
 *
 * @returns Returns true if BOTH sides are on target
 */
void ChassisGoToGoalCompletion(int left, int right)
{
	PIDControllerSetGoal(&leftController, left);
	PIDControllerSetGoal(&rightController, right);

	int goodCount = 0;
	// Use 1 '&' instead of 2 to force execution of both sides (both right and left side will run) because short-circuiting
	while (goodCount < 50)
	{
		if (PIDControllerExecuteContinuous(&leftController) & PIDControllerExecuteContinuous(&rightController))
			goodCount++;
		delay(5);
	}
}

/**
 * @brief Resets the chassis IMEs
 */
void ChassisResetIMEs()
{
	imeReset(I2C_MOTOR_CHASSIS_LEFT);
	imeReset(I2C_MOTOR_CHASSIS_RIGHT);
}

/**
 * @brief Aligns robot to a kTile tile intially going provided speeds.
 *
 * @param left
 *			The initial motor speed for the left side of the chassis
 *
 * @param right
 *			The intial motor speed for the right side of the chassis
 *
 * @param tile
 *			The tile that the line is detected on
 */
void ChassisAlignToLine(int left, int right, kTiles tile)
{
	bool hasPassedLeft = false, hasPassedRight = false;
	bool hasHadLeft = false, hasHadRight = false;
	int goodCount = 0;
	/*
	ChassisSet(left, right, false);
	while (!ChassisHasIRLineRight(tile) && !ChassisHasIRLineLeft(tile)) 
		lcdprintf(Centered, 2, "%d %d", ChassisHasIRLineRight(tile), ChasssisHasIRLineLeft(tile));
	*/
	while (goodCount < 20)
	{
		if (ChassisHasIRLineRight(tile) && !hasHadRight)
		{
			hasPassedRight = !hasPassedRight;
			hasHadRight = true;
		}
		else if (!ChassisHasIRLineRight(tile)) hasHadRight = false;

		if (ChassisHasIRLineLeft(tile) && !hasHadLeft)
		{
			hasPassedLeft = !hasPassedLeft;
			hasHadLeft = true;
		}
		else if (!ChassisHasIRLineLeft(tile)) hasHadLeft = false;

		if (ChassisHasIRLineLeft(tile))
			ChassisSetLeft(0, false);
		else
			ChassisSetLeft(left * (hasPassedLeft ? -1 : 1), false);
		
		if (ChassisHasIRLineRight(tile))
			ChassisSetRight(0, false);
		else
			ChassisSetRight(right * (hasPassedRight ? -1 : 1), false);

		if (ChassisHasIRLineLeft(tile) && ChassisHasIRLineRight(tile)) goodCount++;

		lcdprintf(Centered, 2, "il:%04d r: %04d", ChassisGetIRRight(), ChassisGetIRLeft());
		delay(5);
	}
	ChassisSet(0, 0, false);
}

/**
* @brief Initializes the chassis motors with the SML and creates the PID controllers for the
*        chassis.
*/
void ChassisInitialize()
{
	MotorConfigure(MOTOR_CHASSIS_FRONTLEFT, true, CHASSIS_SKEW_PROFILE);
	MotorConfigure(MOTOR_CHASSIS_FRONTRIGHT, false, CHASSIS_SKEW_PROFILE);
	MotorConfigure(MOTOR_CHASSIS_REARLEFT, false, CHASSIS_SKEW_PROFILE);
	MotorConfigure(MOTOR_CHASSIS_REARRIGHT, false, CHASSIS_SKEW_PROFILE);

	//										Execute			Get					  Kp   Ki     Kd   MaxI MinI Tol
	leftController = PIDControllerCreate(&ChassisSetLeft, &ChassisGetIMELeft,	 0.07, 0.25, 0, 100, -100, 15);
	rightController = PIDControllerCreate(&ChassisSetRight, &ChassisGetIMERight, 0.07, 0.25, 0, 100, -100, 15);

	//gyro = gyroInit(ANA_GYROSCOPE, 196);
}