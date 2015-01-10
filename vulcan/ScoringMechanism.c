/****************************************************************************************/
/* @file vulcan/ScoringMechanism.c		@brief Source file for ScoringMech functions	*/
/*																						*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.					*/
/* Portions of this file may contain elements from the PROS API.						*/
/* See include/API.h for additional notice.												*/
/****************************************************************************************/

#include "main.h"
#include "vulcan/ScoringMechanism.h"
#include "vulcan/CortexDefinitions.h"

/**
 * Sets the scoring mechanism to the desired state 
 * @param value
 *			The output mode of the solenoid true is ACTIVATED/OUT false is DEACTIVATED/IN
 */
void ScoringMechSet(bool value)
{
	digitalWrite(DIG_SCORINGMECH, value);
}

/**
 * Switches the scoring mechanism. If the solenoid is currently activated, it will become deactivated and vice versa
 */
void ScoringMechSwitch()
{
	digitalWrite(DIG_SCORINGMECH, !digitalRead(DIG_SCORINGMECH));
}