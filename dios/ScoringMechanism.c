/************************************************************************/
/* @file ScoringMechanism.c												*/
/* @brief Source file for scoring mech functions.						*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "dios/ScoringMechanism.h"
#include "dios/CortexDefinitions.h"
#include "sml/SmartMotorLibrary.h"

/**
 * Sets the speed of the scoring mechanism motor
 * @param value
 *			[-127,127] The speed of the motor
 * @param immediate
 *			Determines if speed input change is immediate or ramped according to SML
 */
void ScoringMechSet(int value, bool immediate)
{
	MotorSet(MOTOR_SCORINGMECH, value, immediate);
}

/**
 * Initializes the scoring mechanism motor and PID controller
 */
void ScoringMechInitialize()
{
	MotorConfigure(MOTOR_SCORINGMECH, true, DEFAULT_SKEW);
}