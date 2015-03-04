/**
 * @file vulcan/init.c		
 * @brief Source file for initialize functions.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"

#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"
#include "lcd/LCDManager.h"

#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"
#include "vulcan/LCDDisplays.h"


/**
 * @brief Runs pre-initialization functions.
 */
void initializeIO() {
	pinMode(DIG_SCORINGMECH_NEEDLE, OUTPUT);
	pinMode(DIG_SCORINGMECH_CLAW, OUTPUT);
	ScoringMechClawSet(false);
	ScoringMechNeedleSet(true);
	setTeamName("7701");
}


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
	lcdprint_d(Left, 2, 500, "....complete....");
}