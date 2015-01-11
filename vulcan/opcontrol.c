/**
 * @file vulcan/opcontrol.c
 * @brief Source file for operator control.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/
#include <math.h>

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"


/**
 * Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	while (true)
	{
		//ChassisSet(joystickGetAnalog(1,3), joystickGetAnalog(1,2), false);
		//JoystickControl();
		
		LiftSetLeft(joystickGetAnalog(1, 3), false);
		LiftSetRight(joystickGetAnalog(1, 2), false);

		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-127);
		else
			LiftSet(0);
		
		delay(100);
	}
}