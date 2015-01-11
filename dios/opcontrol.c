/**
 * @file dios/opcontrol.c
 * @brief Source file for operator control.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ************************************************************************/
#include <math.h>

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

#include "dios/CortexDefinitions.h"
#include "dios/Chassis.h"
#include "dios/Lift.h"
#include "dios/buttons.h"


/**
 * @brief Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	bool wasPressed = false;
	bool inverted = true;
	while (true)
	{
		if (!wasPressed && joystickGetDigital(1, 8, JOY_UP))
		{
			wasPressed = true;
			inverted = !inverted;
		}
		else if (!joystickGetDigital(1, 8, JOY_UP))
			wasPressed = false;

		ChassisSet((inverted ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (inverted ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), false);
		
		if (joystickGetDigital(1, 5, JOY_UP))
			LiftSetLeft(127, false);
		else if (joystickGetDigital(1, 5, JOY_DOWN))
			LiftSetLeft(-127, false);
		else if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-127);
		else
			LiftSet(0);

		digitalWrite(DIG_SCORINGMECH, !joystickGetDigital(1, 8, JOY_DOWN));

		delay(100);
	}
}