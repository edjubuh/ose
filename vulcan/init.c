/**
 * @file vulcan/init.c
 * @brief Source file for initialize functions.
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#include "main.h"

#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"
#include "lcd/LCDManager.h"
#include "lcd/lcdmenu.h"

#include "vulcan/AutonomousHelper.h"
#include "vulcan/buttons.h"
#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/LCDDisplays.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"


/**
 * @brief Runs pre-initialization functions.
 */
void initializeIO()
{
	pinMode(DIG_SCORINGMECH_NEEDLE, OUTPUT);
	pinMode(DIG_SCORINGMECH_CLAW, OUTPUT);
	ScoringMechClawSet(false);
	ScoringMechNeedleSet(true);
	setTeamName("7701");
}

/**
 * @brief Declare global variables needed for the LCDMenu.
 * 
 * @note GLOBAL VARIABLES DECLARED IN LCDDisplays.h!!!
 */
char *titles[NUMTITLES] = { "No auton", "Blue Sky", "Blue Cube", "Red Sky", "Red Cube", "P. skills" };
void(*exec[NUMTITLES])() = { RunNoAutonomous, RunBlueSky, RunBlueCube, RunRedSky, RunRedCube, RunPSkills };
LCDMenu main_menu;

/**
 * @brief Initializes the robot. Displays graphics depicting process in initialization
 *        sequence.
 */
void initialize()
{
	lcdInitialize();
	lcdprint(Centered, 1, "Booting Vulcan");
	lcdprint(Left, 2, "IMEs... "); // IMES must be first, followed by MotorManager. Chassis and Lift are not order dependent
	imeInitializeAll();
	delay(100);
	lcdprint(Left, 2, "MotorManager... ");
	InitializeMotorManager();
	delay(100);
	lcdprint(Left, 2, "Chassis... ");
	ChassisInitialize();
	delay(100);
	lcdprint(Left, 2, "Lift... ");
	LiftInitialize();
	delay(200);
	lcdprint(Left, 2, "LCD Display...");
	//DisplayText o = { &getRobotState, Left };
	//addCycleText(o, 1);
	lcdprintf(Centered, 1, "E:%1.1fV M:%1.1fV", (double)analogRead(ANA_POWEREXP)/70.0, (double)powerLevelMain()/1000.0); /// @todo double check power expander reading is correct
	lcdprint_d(Left, 2, 500, "....complete....");
	if (!isEnabled() && isOnline())
	{
		lcdprint_d(Left, 1, 500, "Competition mode");
		main_menu = lcdmenuCreate(NUMTITLES, titles, exec);
		bool quit = false;
		lcdprint(Centered, 1, "Select auton");
		lcdmenuDisplay(&main_menu);
		while (!quit)
		{
			if (buttonIsNewPress(LCD_LEFT))
				lcdmenuShift(&main_menu, LCD_SHIFT_LEFT);
			else if (buttonIsNewPress(LCD_CENT))
			{
				if (buttonIsNewPress(LCD_LEFT))
					lcdmenuShift(&main_menu, LCD_SHIFT_LEFT);
				else if (buttonIsNewPress(LCD_CENT))
				{
					lcdmenuDecide(&main_menu);
					quit = true;
					continue;
				}
				else if (buttonIsNewPress(LCD_RIGHT))
					lcdmenuShift(&main_menu, LCD_SHIFT_RIGHT);
				//delay and allow for other tasks to take place
				delay(100);
			}
			lcdprint(Centered, 1, "Selected");
            //print the selected autonomous
            lcdprintf(Centered, 2, "%s", main_menu.titles[main_menu.execute]);
			delay(750);
		}
	}
}