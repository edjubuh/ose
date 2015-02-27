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

#define GREY_WHITE_LINE_THRESH	600
#define BLUE_WHITE_LINE_THRESH	450
#define RED_WHITE_LINE_THRESH	300

static int skyriseBuilt = 0;
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
	delay(600);
	ChassisSet(127, 127, false);
	delay(100);
	ChassisSet(0, 0, false);
	ScoringMechClawSet(true);
	LiftGoToHeightCompletion(12);
	ChassisSet(-127, -127, false);
	LiftSetHeight(0);
	delay(500);
	ChassisSet(-35, -35, false);
	while (ChassisGetIRRight() > GREY_WHITE_LINE_THRESH && ChassisGetIRLeft() > 900) delay(10);
	ChassisSet(0, 0, false);
	ChassisSet(20, 20, false);
	while (ChassisGetIRRight() > GREY_WHITE_LINE_THRESH && ChassisGetIRLeft() > 900) delay(10);
	ChassisSet(0, 0, false);
	ScoringMechClawSet(false);
	lcdprintf(Centered, 2, "Finished %.2f", (millis() - start)/1000.0);
}

/**
 * @brief Builds the next skyrise
 */
void BuildSkyrise()
{

}