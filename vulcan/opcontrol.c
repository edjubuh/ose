<<<<<<< HEAD
/**
 * @file vulcan/opcontrol.c
 * @brief Source file for operator control.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/
=======
/*****************************************************************************/
/* @file vulan/opcontrol.c		@brief Source file for operator control.	*/
/*																			*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.		*/
/* Portions of this file may contain elements from the PROS API.			*/
/* See include/API.h for additional notice.									*/
/****************************************************************************/
>>>>>>> 7b38228c15fb78daf33c4b4aee5eb4dcd083a15a
#include <math.h>

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"
<<<<<<< HEAD

#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"


/**
 * Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
=======

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
>>>>>>> 7b38228c15fb78daf33c4b4aee5eb4dcd083a15a
	while (true)
	{
		//ChassisSet(joystickGetAnalog(1,3), joystickGetAnalog(1,2), false);
		//JoystickControl();
		
		LiftSetLeft(joystickGetAnalog(1, 3), false);
		LiftSetRight(joystickGetAnalog(1, 2), false);
<<<<<<< HEAD
		
		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-127);
=======
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
>>>>>>> 7b38228c15fb78daf33c4b4aee5eb4dcd083a15a
		else
			LiftSet(0);
		
		delay(100);
	}
}