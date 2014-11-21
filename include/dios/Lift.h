/************************************************************************/
/* @file lift.h		@brief Header file for lift functions				*/
/* See src/lift.c for details of all functions                       */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef LIFT_H_
#define LIFT_H_

#include "sml/SmartMotorLibrary.h"

/// <summary>
/// Sets the lift PWM output to the value
/// </summary>
/// <param name='value'>The PWM value. Will be checked for bounds</param>
/// <param name='immediate'>Indicates if PWM change needs to happen immediately. Usually false, will ramp speeds</param>
void LiftSet(int value, bool immediate);

//void LiftSetLeft(int, bool);

//void LiftSetRight(int, bool);

void LiftSetHeight(int value);

void LiftInitialize();

PIDController LiftPIDController_l, LiftPIDController_r;

#endif