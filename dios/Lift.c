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

static MasterSlavePIDController Controller;
static TaskHandle LiftControllerTask;

/**
* Sets the lift to the desired speed
* @param value
*			[-127,127] Speed of the lift
* @param immediate
*			Determines if speed input change is immediate or ramped according to SML
*/
void LiftSet(int value, bool immediate)
{
	MotorSet(MOTOR_LIFT_FRONTLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_FRONTRIGHT, value, immediate);
	MotorSet(MOTOR_LIFT_REARLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_REARRIGHT, value, immediate);
	MotorSet(MOTOR_LIFT_THLEFT, value, immediate);
	MotorSet(MOTOR_LIFT_THRIGHT, value, immediate);
}

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
	return value;
}

/**
* Initializes the lif
t motors and PID controllers
*/
void LiftInitialize()
{
	MotorConfigure(MOTOR_LIFT_FRONTLEFT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_REARLEFT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_REARRIGHT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_THLEFT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_THRIGHT, false, DEFAULT_SKEW);
	
	PIDController master = PIDControllerCreate(&LiftSetRight, &LiftGetEncoderRight, 1.00, 0.1, 0.001, 100, -100, 10);
	PIDController slave = PIDControllerCreate(&LiftSetLeft, &LiftGetEncoderLeft, 1.00, 0.1, 0.001, 100, -100, 10);
	
	Controller = CreateMasterSlavePIDController(master, slave);
	
	LiftControllerTask = InitializeMasterSlaveController(&Controller, 0);
}