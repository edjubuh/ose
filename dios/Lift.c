/**
 * @file lift.c
 * @brief Source file for lift functions.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ************************************************************************/

#include "main.h"
#include "dios/Lift.h"
#include "dios/CortexDefinitions.h"
#include "sml/SmartMotorLibrary.h"
#include "sml/MasterSlavePIDController.h"
#include "sml/SingleThreadPIDController.h"

static MasterSlavePIDController Controller;
static TaskHandle LiftControllerTask;



/**
* @brief Sets the left side of the lift to the desired speed.
*
* @param value
*        [-127,127] Speed of the lift
* @param immediate
*        Determines if speed input change is immediate or ramped according to SML.
*/
static void LiftSetLeft(int value, bool immediate)
{
	MotorSet(MOTOR_LIFT_FRONTLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_REARLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_THLEFT, value, immediate);
}

/**
* @brief Sets the right side of the lift to the desired speed.
*
* @param value
*        [-127,127] Speed of the lift
* @param immediate
*        Determines if speed input change is immediate or ramped according to SML.
*/
static void LiftSetRight(int value, bool immediate)
{
	MotorSet(MOTOR_LIFT_FRONTRIGHT, value, immediate);
	MotorSet(MOTOR_LIFT_REARRIGHT, value, immediate);
	MotorSet(MOTOR_LIFT_THRIGHT, value, immediate);
}


/**
 * @brief Sets the lift to the desired speed.
 * @note Requires LiftSetLeft() and LiftSetRight() below.
 *
 * @param value
 *        [-127,127] Speed of the lift
 * @param immediate
 *        Determines if speed input change is immediate or ramped according to SML.
 */
void LiftSet(int value, bool immediate)
{
	LiftSetLeft(value,  immediate);
	LiftSetRight(value, immediate);
}


/**
 * @brief Returns the value of motor I2C_MOTOR_LIFT_RIGHT.
 *
 * @return The motor count of motor I2C_MOTOR_LIFT_RIGHT.
 */
static int LiftGetEncoderRight()
{
	int value;
	imeGet(I2C_MOTOR_LIFT_RIGHT, &value);
	return value;
}
/**
 * @brief Returns the value of motor I2C_MOTOR_LIFT_LEFT.
 *
 * @return The motor count of motor I2C_MOTOR_LIFT_LEFT.
 */
static int LiftGetEncoderLeft()
{
	int value;
	imeGet(I2C_MOTOR_LIFT_LEFT, &value);
	return value;
}

void LiftSetHeight(int value)
{
	MasterSlavePIDChangeGoal(&Controller, value);
}

/**
* Initializes the lift motors and PID controllers
*/
void LiftInitialize()
{
	MotorConfigure(MOTOR_LIFT_FRONTLEFT,  true,  DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT, true,  DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_REARLEFT,   true,  DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_REARRIGHT,  true,  DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_THLEFT,     true,  DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_THRIGHT,    false, DEFAULT_SKEW);
	
	PIDController master = PIDControllerCreate(&LiftSetRight, &LiftGetEncoderRight, 1.00, 0.1, 0.001, 100, -100, 10);
	PIDController slave  = PIDControllerCreate(&LiftSetLeft,  &LiftGetEncoderLeft,  1.00, 0.1, 0.001, 100, -100, 10);
	
	Controller = CreateMasterSlavePIDController(master, slave);
	
	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
}