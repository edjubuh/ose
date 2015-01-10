/*****************************************************************************/
/* @file vulan/opcontrol.c		@brief Source file for operator control.	*/
/*																			*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.		*/
/* Portions of this file may contain elements from the PROS API.			*/
/* See include/API.h for additional notice.									*/
/****************************************************************************/

#include "main.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"
#include "vulcan/buttons.h"

void operatorControl()
{
	lcdSetText(uart1, 2, "OSE Smiles");
	bool Btn6DWasPressed = false, current = false;
	while (true)
	{
		ChassisSet(joystickGetAnalog(1, 3), joystickGetAnalog(1, 2), false);

		if (joystickGetDigital(1, 5, JOY_DOWN) && !Btn6DWasPressed)
		{
			Btn6DWasPressed = true;
			lcdSetText(uart1, 2, "Button Pressed");
			digitalWrite(10, current = !current);
		}
		else if (!joystickGetDigital(1, 5, JOY_DOWN))
			Btn6DWasPressed = false;

		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127, false);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-70, false);
		else
			LiftSet(0, false);
	}
}