/**
 * @file vulcan/ScoringMechanism.c
 * @brief Source file for ScoringMech functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "vulcan/ScoringMechanism.h"

#include "vulcan/CortexDefinitions.h"

/**
 * Sets the scoring claw to the desired state 
 * @param value
 *			The output mode of the solenoid true is RELEASE/OUT false is ACTIVE/IN
 */
void ScoringMechClawSet(bool value)
{
	digitalWrite(DIG_SCORINGMECH_CLAW, value);
}

/**
 * Switches the scoring claw. If the solenoid is currently activated, it will become deactivated and vice versa
 */
void ScoringMechClawSwitch()
{
	digitalWrite(DIG_SCORINGMECH_CLAW, !digitalRead(DIG_SCORINGMECH_CLAW));
}


/**
* Sets the scoring mechanism to the desired state
* @param value
*			The output mode of the solenoid true is RELEASE/OUT false is ACTIVE/IN
*/
void ScoringMechNeedleSet(bool value)
{
	digitalWrite(DIG_SCORINGMECH_NEEDLE, value);
}

/**
* Switches the scoring mechanism. If the solenoid is currently activated, it will become deactivated and vice versa
*/
void ScoringMechNeedleSwitch()
{
	digitalWrite(DIG_SCORINGMECH_NEEDLE, !digitalRead(DIG_SCORINGMECH_NEEDLE));
}