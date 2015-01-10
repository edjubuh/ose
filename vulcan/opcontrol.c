<<<<<<< HEAD
/*****************************************************************************/
/* @file vulan/opcontrol.c		@brief Source file for operator control.	*/
/*																			*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.		*/
/* Portions of this file may contain elements from the PROS API.			*/
/* See include/API.h for additional notice.									*/
/****************************************************************************/
=======
/**
 * @file opcontrol.c
 * @brief Source file for operator control.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ************************************************************************/
#include <math.h>
>>>>>>> e5c9ddddc0a530397ad087ee1f230d2c22dfc836

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

#include "dios/CortexDefinitions.h"
#include "dios/Chassis.h"
#include "dios/Lift.h"


/**
 * @brief Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	/*
	char ln1[16];
	char ln2[16];
	for (int i = 0; i < 16; i++)
	{
		ln1[i] = ' ';
		ln2[i] = ' ';
	}
	*/
	while (true)
	{
		//ChassisSet(joystickGetAnalog(1,3), joystickGetAnalog(1,2), false);
		//JoystickControl();
		
		LiftSetLeft(joystickGetAnalog(1, 3), false);
		LiftSetRight(joystickGetAnalog(1, 2), false);
		/*
		sprintf(ln1, "left: %d", LiftGetCalibratedPotentiometerLeft());
		sprintf(ln2, "right: %d", LiftGetCalibratedPotentiometerRight());

		printText(ln1, Left, 1);
		printText(ln2, Right, 2);
		*/
		
		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(20);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-20);
		else
			LiftSet(0);
		
		delay(100);
	}
}