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
* @brief Deploys the Scoring Mechanism by lifting the lift to 15 ticks, and then lowers back to 0
*/
void DeployScoringMech()
{
	LiftGoToHeightCompletion(15);
	delay(50);
	ScoringMechClawSet(false);
	LiftGoToHeightCompletion(0);
}

/**
* @brief Builds the next skyrise
*/
void BuildSkyrise()
{
	ChassisResetIMEs();
	if (skyriseBuilt == 0)
	{
		ChassisSet(127, 127, false);
		delay(60);
		ChassisSet(0, 0, false);
	}
	ScoringMechClawSet(true);
	LiftGoToHeightCompletion(15);
	delay(25);
	ChassisSet(-60, -60, false);
	delay(150);
	int height = 0;
	switch (skyriseBuilt)
	{
		case 0:
			height = 0;
			break;
		case 1:
			height = 20;
			break;
	}
	while (!LiftGoToHeightContinuous(height) || !ChassisGoToGoalContinuous(-750, -750)) delay(10);
	ChassisAlignToLine(-30, -30, Grey);
	delay(25);
	ChassisSetMecanum(-HALFPI, 127, 0, false);
	delay(110);
	ChassisSet(0, 0, false);
	delay(600);
	ScoringMechClawSet(false);
	delay(200);
	ChassisSet(127, 127, false);
	delay(250);
	LiftGoToHeightContinuous(0);
	ChassisGoToGoalCompletion(15, 30);
	ScoringMechClawSet(true);
	skyriseBuilt++;
}

/**
 * @brief Runs a sequence of commands during the competition "autonomous period." (15 seconds)
 */
void autonomous()
{
	lcdprint(Centered, 2, "Running auton");
	long start = millis();
	DeployScoringMech();
	BuildSkyrise();
	BuildSkyrise();
	/*
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
	*/
	lcdprint_df(Centered, 2, 1000, "Finished %.2f", (millis() - start)/1000.0);
}