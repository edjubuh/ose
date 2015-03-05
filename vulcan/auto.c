/**
 * @file vulcan/auto.c
 * @brief Source file for autonomous functions.
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
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
* @brief Builds the next skyrise from the red tile
*/
void BuildSkyrise()
{
	// Drive forward a little to ensure touching the skyrise
	ChassisSet(127, 127, false);
	delay(65);
	ChassisSet(0, 0, true);
	delay(80);
	ChassisResetIMEs();
	ScoringMechClawSet(true);
	// Go up to avoid skyrise base
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
		// Back up for at least 600 milliseconds to get off of red tile
		if (millis() - start > 600)
			ChassisSet(0, 0, false);
		delay(10);
	}
	if (millis() - start < 600) delay(600 - (millis() - start)); // Finish up remaining 600 milliseconds if necessary
	ChassisSet(0, 0, false);
	delay(100);
	ChassisAlignToLine(-30, -30, Grey); // Align self to line to ready for drop
	delay(25);
	ChassisSetMecanum(-HALFPI, 127, 1, false);
	//delay(150);
	if(skyriseBuilt == 0) delay(135);
	else delay(230);
	ChassisSet(0, 0, true);
	if (skyriseBuilt > 1)
	{ // Once we build one skyrise, need to go forward a little to align correctly
		ChassisSet(127, 127, false);
		delay(50);
		ChassisSet(0, 0, true);
	}
	delay(550); // Wait for robot to settle
	ScoringMechClawSet(false);
	delay(300); // Wait for skyrise to drop far enough for robot to begin driving forward
	ChassisSetMecanum(HALFPI, 127, 0, false);
	delay(45);
	ChassisSet(0, 0, true);
	delay(50);
	//delay(1000);
	ChassisResetIMEs();
	ChassisSet(127, 127, false);
	delay(250);
	LiftGoToHeightContinuous(0);
	ChassisGoToGoalCompletion(1035, 1035); // return to base tile
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
	lcdprint_df(Centered, 2, 2000, "Finished %.2f", (millis() - start)/1000.0);
}