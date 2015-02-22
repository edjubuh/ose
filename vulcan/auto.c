/**
 * @file vulcan/auto.c		
 * @brief Source file for autonomous functions.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "lcd/LCDFunctions.h"

#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"

/**
 * @brief Runs a sequence of commands during the competition "autonomous period." (15 seconds)
 */
void autonomous()
{
	lcdprint(Centered, 2, "Running auton");
	long start = millis();
	LiftGoToHeightCompletion(8);
	delay(50);
	ScoringMechClawSet(false);
	LiftGoToHeightCompletion(0);
	delay(100);
	ScoringMechClawSet(true);
	LiftGoToHeightCompletion(10);
	lcdprintf(Centered, 2, "Finished %f", (millis() - start)/1000.0);
}