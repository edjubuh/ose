/**
 * @file dios/init.c
 * @brief Source file for initialize functions.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ************************************************************************/

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

#include "dios/CortexDefinitions.h"
#include "dios/Chassis.h"
#include "dios/Lift.h"
#include "dios/buttons.h"


/**
 * @brief Runs pre-initialization functions.
 */
void initializeIO() {
	pinMode(1, OUTPUT);
	pinMode(4, OUTPUT);
	setTeamName("7701");
}


/**
 * @brief Initializes the robot. Displays graphics depicting process in initialization
 *        sequence.
 */
void initialize()
{
	InitializeMotorManager();
	ChassisInitialize();
	LiftInitialize();
	initButtons();
}