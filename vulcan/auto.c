/************************************************************************/
/* @file vulcan/auto.c		@brief Source file for autonomous functions.*/
/*														                */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "vulcan/Chassis.h"

/**
 * Runs a sequence of commands during the competition "autonomous period." (15 seconds)
 */
void autonomous()
{
	lcdSetText(uart1, 2, "HULK SMASH");
	ChassisSet(-127, -127, false);
	delay(4000);
	ChassisSet(0, 0, false);
}