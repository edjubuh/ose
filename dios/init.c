/**
 * @file init.c
 * @brief Source file for initialize functions.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ************************************************************************/

#include "main.h"
#include "dios/CortexDefinitions.h"
#include "sml/SmartMotorLibrary.h"
#include "dios/Chassis.h"
#include "dios/Lift.h"


/**
 * @brief Runs pre-initialization functions.
 */
void initializeIO() {
	setTeamName("7702");
}


/**
 * @brief Initializes the robot. Displays graphics depicting process in initialization
 *        sequence.
 */
void initialize()
{
	lcdInit(uart1);
	lcdSetBacklight(uart1, true);
	lcdClear(uart1);
	lcdSetText(uart1, 1, "  Booting Dios  ");
	lcdSetText(uart1, 2, "...");
	InitializeMotorManager();
	lcdSetText(uart1, 2, "......");
	ChassisInitialize();
	lcdSetText(uart1, 2, ".......OSE");
	LiftInitialize();
	lcdSetText(uart1, 2, ".............");
	lcdSetText(uart1, 2, "........OSE......");
	delay(250);
	lcdClear(uart1);
	lcdSetText(uart1, 1, "  Boot Complete  ");
	lcdSetText(uart1, 2, "Dios | 7702 | OSE");
	delay(500);
}