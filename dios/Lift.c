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
}

/**
* Initializes the lift motors and PID controllers
*/
void LiftInitialize()
{
	MotorConfigure(MOTOR_LIFT_FRONTLEFT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT, true, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_REARLEFT, false, DEFAULT_SKEW);
	MotorConfigure(MOTOR_LIFT_REARRIGHT, true, DEFAULT_SKEW);
}