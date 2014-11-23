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

#define IME_RESET_THRESHOLD		100

static MasterSlavePIDController Controller;
static TaskHandle LiftControllerTask;



void LiftSetRight(int value, bool immediate)
{
	MotorSet(MOTOR_LIFT_FRONTRIGHT, value, immediate);
	MotorSet(MOTOR_LIFT_REARRIGHT, value, immediate);
	MotorSet(MOTOR_LIFT_THRIGHT, value, immediate);
}

int LiftGetEncoderRight()
{
	int value;
	imeGet(I2C_MOTOR_LIFT_RIGHT, &value);
	value = (int)((-0.9783 * value) + 55.091);
	if(value < IME_RESET_THRESHOLD && digitalRead(DIG_LIFT_BOTLIM_RIGHT) == LOW)
	{
		imeReset(I2C_MOTOR_LIFT_RIGHT);
		value = 0;
	}
	return value;
}

void LiftSetLeft(int value, bool immediate)
{
	MotorSet(MOTOR_LIFT_FRONTLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_REARLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_THLEFT, value, immediate);
}

int LiftGetEncoderLeft()
{
	int value;
	imeGet(I2C_MOTOR_LIFT_LEFT, &value);
	if(value < IME_RESET_THRESHOLD && digitalRead(DIG_LIFT_BOTLIM_LEFT) == LOW)
	{
		imeReset(I2C_MOTOR_LIFT_LEFT);
		value = 0;
	}
	return value;
}

/**
* Sets the lift to the desired speed
* @param value
*			[-127,127] Speed of the lift
* @param immediate
*			Determines if speed input change is immediate or ramped according to SML
*/
void LiftSet(int value)
{
	MasterSlavePIDSetOutput(&Controller, value);
	//LiftSetRight(value, false);
	//LiftSetLeft(value, false);
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
	
	PIDController master = PIDControllerCreate(&LiftSetRight, &LiftGetEncoderRight, 1.00, 0.1, 0.001, 100, -100, 2);
	PIDController slave = PIDControllerCreate(&LiftSetLeft, &LiftGetEncoderLeft, 0.5, 0, 0.2, 40, -40, 2);
	
	Controller = CreateMasterSlavePIDController(master, slave, false);
	
	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
}

void LiftSetHeight(int goal)
{
	MasterSlavePIDChangeGoal(&Controller, goal);
}