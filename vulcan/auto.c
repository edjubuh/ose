/**
 * @file vulcan/auto.c
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
#include "vulcan/LCDDisplays.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"

#define GREY_WHITE_LINE_THRESH	600
#define BLUE_WHITE_LINE_THRESH	450
#define RED_WHITE_LINE_THRESH	300

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
	LiftGoToHeightContinuous(4);
	ChassisSet(127, 127, false);
	delay(100);
	ChassisSet(0, 0, true);
	delay(80);
    
    //Reset the IMEs
	ChassisResetIMEs();
    //extend claw
	ScoringMechClawSet(true);
    
	// Go up to avoid skyrise base
	LiftGoToHeightCompletion(15);
	delay(25);
    
    /**
     * @note Go to specified height at specified speed to build the next
     * Skyrise section. skyriseBuilt is an extern variable.
     */
	int height = 0;
	int speed = -127;
	switch (skyriseBuilt)
	{
		case 0:
			height = 0;
			speed = -127;
			break;
		case 1:
			height = 19;
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
		// Back up for at least 700 milliseconds to get off of red tile
		if (millis() - start > 800)
			ChassisSet(0, 0, false);
		delay(10);
	}
	if (millis() - start < 800)
        delay(800 - (millis() - start)); // Finish up remaining 600 milliseconds if necessary
    
	ChassisSet(0, 0, false);
	delay(100);
	ChassisAlignToLine(-30, -30, Grey); // Align self to line to ready for drop
	delay(25);
	ChassisSetMecanum(-M_PI_2, 127, 1, false);
	//delay(150);
    
	if(skyriseBuilt == 0)
       delay(250);
	else if (skyriseBuilt <= 1)
        delay(285);
	else
        delay(280);
    
	ChassisSet(0, 0, true);
	if (skyriseBuilt == 0)
	{
		ChassisSet(-127, -127, false);
		delay(40);
		ChassisSet(0, 0, true);
	}
	if (skyriseBuilt > 1) {
        // Once we build one skyrise, need to go forward a little to align correctly
		ChassisSet(127, 127, false);
		delay(40);
		ChassisSet(0, 0, true);
	}
	
	delay(550); // Wait for robot to settle
	ScoringMechClawSet(false);
	if (skyriseBuilt == 1)
		LiftGoToHeightContinuous(5);
	delay(300); // Wait for skyrise to drop far enough for robot to begin driving forward
	if (skyriseBuilt == 1)
		LiftGoToHeightContinuous(15);
	if (skyriseBuilt <= 1)
	{
		ChassisSet(-127, -127, false);
		delay(50);
		ChassisSet(0, 0, false);
	}
	ChassisSetMecanum(M_PI_2, 127, 0, false);
	if(skyriseBuilt == 0) delay(120);
	else delay(80);
	ChassisSet(0, 0, true);
	delay(50);
	//delay(1000);
	ChassisResetIMEs();
	ChassisSet(127, 127, false);
	delay(250);
	LiftGoToHeightContinuous(0);
	ChassisGoToGoalCompletion(1050, 1050); // return to base tile
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
	ChassisResetIMEs();
	lcdmenuExecute(&main_menu);
#ifdef AUTO_DEBUG
	lcdprint_df(Centered, 2, 2000, "Finished %.2f", (millis() - start)/1000.0);
#endif
}

/**
 * @brief Runs "no autonomous" autonomous for use when requested by teams or autonomous
 *        should not function. Will deploy scoring mechanism anyway.
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
	LiftGoToHeightCompletion(15);
	ChassisGoToGoalCompletion(1800, 1800);
	LiftGoToHeightCompletion(0);
	delay(300);
	LiftGoToHeightCompletion(90);
	ChassisResetIMEs();
	ChassisSetMecanum(-M_PI_2, 127, -2, false);
	delay(750);
	ChassisSet(0, 0, false);
	delay(50);
	ChassisResetIMEs();
	ChassisGoToGoalCompletion(650, -550);
	ChassisSet(127, 127, false);
	delay(650);
	ChassisSet(0, 0, false);
	LiftGoToHeightCompletion(75);
	ScoringMechNeedleSet(false); //drop 2 cubes
	delay(1200);
	LiftGoToHeightCompletion(90);
	ChassisSet(-127, -127, false);
	delay(800);
	ChassisSet(0, 0, false);
	LiftGoToHeightCompletion(0);
}

/**
 * @brief Runs the blue cube autonomous starting at the blue starting tile next to the autoloader
 */
void RunBlueCube()
{
	LiftGoToHeightCompletion(50);
	ChassisSetMecanum(-M_PI_2, 127, -2, false);
	delay(1300);
	ChassisSet(0, 0, false);
	ScoringMechNeedleSet(false);
	delay(400);
	ScoringMechNeedleSet(true);
	ChassisSetMecanum(M_PI_2, 127, 3, false);
	delay(2000);
	LiftGoToHeightContinuous(0);
	ChassisSet(127, -127, false);
	delay(175);
	ChassisSet(0, 0, false);
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
	LiftGoToHeightCompletion(50);
	//ChassisSet(127, 127, false);
	//delay(50);
	ChassisSetMecanum(M_PI_2, 127, 2, false);
	delay(1400);
	ChassisSet(0, 0, false);
	ScoringMechNeedleSet(false);
	delay(500);
	ChassisSetMecanum(-M_PI_2, 127, -3, false);
	delay(2000);
	LiftGoToHeightContinuous(0);
	ChassisSet(-127, 127, false);
	ScoringMechNeedleSet(true);
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
	/*BuildSkyrise();
	delay(9000);
	
	ChassisSet(-127, -127, false);
	delay(6000);
	ChassisSet(0, 0, false);
	delay(500);
	ChassisSet(127, 127, false);
	delay(5000);
	ChassisSet(0, 0, false);
	delay(5000);
	ChassisSet(-127, -127, false);
	delay(7500);
	ChassisSet(0, 0, false);
	delay(500);
	ChassisSetMecanum(M_PI_2, 127, 0, false);
	delay(750);
	ChassisSet(127, 127, false);
	delay(7500);
	ChassisSet(0, 0, false);*/
}