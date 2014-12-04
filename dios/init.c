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
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
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
	lcdSetText(uart1, 2, "IMEs... "); // IMES must be first, followed by MotorManager. Chassis and Lift are not order dependent
	imeInitializeAll();
	delay(100);
	lcdSetText(uart1, 2, "MotorManager... ");
	InitializeMotorManager();
	delay(100);
	lcdSetText(uart1, 2, "Chassis... ");
	ChassisInitialize();
	delay(100);
	lcdSetText(uart1, 2, "Lift... ");
	LiftInitialize();
	delay(200);
	lcdSetText(uart1, 2, "");
	delay(100);
	lcdSetText(uart1, 2, "       pl       ");
	delay(100);
	lcdSetText(uart1, 2, "      mple      ");
	delay(100);
	lcdSetText(uart1, 2, "     omplet     ");
	delay(100);
	lcdSetText(uart1, 2, "    complete    ");
	delay(100);
	lcdSetText(uart1, 2, "   .complete.   ");
	delay(100);
	lcdSetText(uart1, 2, "  ..complete..  ");
	delay(100);
	lcdSetText(uart1, 2, " ...complete... ");
	delay(100);
	lcdSetText(uart1, 2, "....complete....");
	delay(100);
	lcdSetText(uart1, 2, "");
	delay(100);
	lcdSetText(uart1, 2, "....complete....");
}