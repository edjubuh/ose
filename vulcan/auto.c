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
#include <math.h>

#include "vulcan/AutonomousHelper.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"

#define AUTO_DEBUG

#define GREY_WHITE_LINE_THRESH	600
#define BLUE_WHITE_LINE_THRESH	450
#define RED_WHITE_LINE_THRESH	300


bool runAutonomous;
bool alliance;
bool startingTile;
bool runPSkills;

int skyriseBuilt = 0;

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
	else if (skyriseBuilt == 1) delay(260);
	else delay(280);
	ChassisSet(0, 0, true);
	if (skyriseBuilt > 1)
	{ // Once we build one skyrise, need to go forward a little to align correctly
		ChassisSet(127, 127, false);
		delay(40);
		ChassisSet(0, 0, true);
	}
	delay(550); // Wait for robot to settle
	ScoringMechClawSet(false);
	delay(300); // Wait for skyrise to drop far enough for robot to begin driving forward
	ChassisSetMecanum(HALFPI, 127, 0, false);
	if(skyriseBuilt == 0) delay(45);
	else delay(80);
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
#ifdef AUTO_DEBUG
	long start = millis();
#endif
	skyriseBuilt = 0;
	lcdmenuExecute(&main_menu);
	
#ifdef AUTO_DEBUG
	lcdprint_df(Centered, 2, 2000, "Finished %.2f", (millis() - start)/1000.0);
#endif
}

/**
 * @brief Runs "no autonomous" autonomous for use when requested by teams or autonomous should not function. Will deploy scoring mechanism anyway
 */
void RunNoAutonomous()
{
	DeployScoringMech();
}

/**
 * @brief Scores two Skyrise pieces from the blue starting tile next to the low post
 */
void RunBlueSky()
{

}

/**
 * @brief Runs the blue cube autonomous starting at the blue starting tile next to the autoloader
 */
void RunBlueCube()
{

}

/**
 * @brief Scores two Skyrise pieces from the red starting tile next to the autoloader
 */
void RunRedSky()
{
	DeployScoringMech();
	BuildSkyrise();
	BuildSkyrise();
}

/**
 * @brief Scores one cube on the low post from the red starting tile next to the low post
 */
void RunRedCube()
{
	LiftGoToHeightCompletion(45);
	//ChassisSet(127, 127, false);
	//delay(50);
	ChassisSetMecanum(M_PI_2, 127, 2, false);
	delay(1400);
	ChassisSet(0, 0, false);
	ScoringMechNeedleSet(false);
	delay(400);
	ScoringMechNeedleSet(true);
	ChassisSetMecanum(-M_PI_2, 127, -3, false);
	delay(2000);
	LiftGoToHeightContinuous(0);
	ChassisSet(-127, 127, false);
	delay(1000);
	ChassisSet(0, 0, false);
}

/**
 * @brief Runs the programming skills routine.
 */
void RunPSkills()
{
	DeployScoringMech();
	BuildSkyrise();
	BuildSkyrise();
	BuildSkyrise();
	ChassisSet(127, -127, false);
	delay(1750);
	ChassisSet(0, 0, false);
	delay(9000);
	ChassisResetIMEs();
	while (!ChassisGoToGoalContinuous(1700, 1700) | !LiftGoToHeightContinuous(15))
		delay(10);
	LiftGoToHeightContinuous(0);
	ChassisSet(0, 0, false);
}