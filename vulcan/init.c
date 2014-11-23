/************************************************************************/
/* @file init.c		@brief Source file for initialize functions.		*/
/*														                */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "vulcan/CortexDefinitions.h"
#include "sml/SmartMotorLibrary.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"
#include "vulcan/buttons.h"

/**
 * Initializes IO pins and sets team name
 */
void initializeIO() {
	setTeamName("7701");
	pinMode(DIG_SCORINGMECH, OUTPUT);
	digitalWrite(DIG_SCORINGMECH, HIGH);
}

/**
 * Initializes the LCD screen, prompts for autonomous selection, and initializes the various subsystems and components
 */
void initialize()
{
	lcdInit(UART_LCDDISPLAY);
	lcdClear(UART_LCDDISPLAY);
	lcdSetBacklight(UART_LCDDISPLAY, true);

	lcdSetText(UART_LCDDISPLAY, 1, " Booting Vulcan ");
	lcdSetText(UART_LCDDISPLAY, 2, "..");
	imeInitializeAll();
	lcdSetText(UART_LCDDISPLAY, 2, ".....");
	InitializeMotorManager();
	lcdSetText(UART_LCDDISPLAY, 2, ".......");
	ChassisInitialize();
	lcdSetText(UART_LCDDISPLAY, 2, "........OS");
	LiftInitialize();
	lcdSetText(UART_LCDDISPLAY, 2, "........OSE.");
	initButtons();
	lcdSetText(UART_LCDDISPLAY, 2, "........OSE......");
	delay(250);
	lcdClear(UART_LCDDISPLAY);
	lcdSetText(UART_LCDDISPLAY, 1, "  Boot Complete  ");
	lcdSetText(UART_LCDDISPLAY, 1, " Vulcan|7701|OSE ");
	delay(500);
}