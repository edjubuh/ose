/**
 * @file vulcan/ScoringMechanism.c
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#include "main.h"
#include "vulcan/ScoringMechanism.h"

#include "vulcan/CortexDefinitions.h"

//------- SCORING NEEDLE ------ //
/**
 * Sets the scoring claw to the desired state 
 * @param value
 *			The output mode of the solenoid true is RELEASE/OUT and false is ACTIVE/IN
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
 * @brief Gets the current state of the scoring claw
 * @returns 
 *			The output mode of the solenoid: true is RELEASE/OUT and false is ACTIVE/IN
 */
bool ScoringMechClawGet()
{
	return digitalRead(DIG_SCORINGMECH_CLAW);
}

//-------- SCORING CLAW ------- //
/**
* Sets the scoring mechanism to the desired state
* @param value
*			The output mode of the solenoid true is RELEASE/OUT and false is ACTIVE/IN
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

/**
* @brief Gets the current state of the scoring needle
* @returns
*			The output mode of the solenoid: true is RELEASE/OUT and false is ACTIVE/IN
*/
bool ScoringMechNeedleGet()
{
	return digitalRead(DIG_SCORINGMECH_NEEDLE);
}