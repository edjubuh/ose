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
	ChassisSet(127, 127, false);
	delay(65);
	ChassisSet(0, 0, true);
	delay(80);
	ChassisResetIMEs();
	ScoringMechClawSet(true);
	LiftGoToHeightCompletion(15);
	delay(25); 
	int height = 0;
	int speed = -127;
	switch (skyriseBuilt)
	{
		case 0:
			height = 0;
			speed = -127;
			break;
		case 1:
			height = 14;
			speed = -127;
			break;
		case 2:
			height = 33;
			speed = -127;
			break;
	}
	ChassisSet(speed, speed, false);
	delay(100);
	long start = millis();
	while (!LiftGoToHeightContinuous(height))
	{
		if (millis() - start > 600)
			ChassisSet(0, 0, false);
		delay(10);
	}
	if (millis() - start < 600) delay(600 - (millis() - start));
	ChassisSet(0, 0, false);
	delay(100);
	ChassisAlignToLine(-30, -30, Grey);
	delay(25);
	ChassisSetMecanum(-HALFPI, 127, 1, false);
	//delay(150);
	if(skyriseBuilt == 0) delay(135);
	else delay(230);
	ChassisSet(0, 0, true);
	if (skyriseBuilt > 1)
	{
		ChassisSet(127, 127, false);
		delay(50);
		ChassisSet(0, 0, true);
	}
	delay(550);
	ScoringMechClawSet(false);
	delay(300);
	ChassisSetMecanum(HALFPI, 127, 0, false);
	delay(45);
	ChassisSet(0, 0, true);
	delay(50);
	//delay(1000);
	ChassisResetIMEs();
	ChassisSet(127, 127, false);
	delay(250);
	LiftGoToHeightContinuous(0);
	ChassisGoToGoalCompletion(1035, 1035);
	ChassisSet(0, 0, true);
	delay(50);
	skyriseBuilt++;
}

/**
 * @brief Runs a sequence of commands during the competition "autonomous period." (15 seconds)
 */
void autonomous()
{
	lcdprint(Centered, 2, "Running auton");
	long start = millis();
	skyriseBuilt = 0;
	DeployScoringMech();
	BuildSkyrise();
	BuildSkyrise();
	//BuildSkyrise();
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
	lcdprint_df(Centered, 2, 2000, "Finished %.2f", (millis() - start)/1000.0);
}