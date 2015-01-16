/**
 * @file vulcan/opcontrol.c
 * @brief Source file for operator control.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

#include "vulcan/buttons.h"
#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"


/**
 * Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	char ln1[16];
	char ln2[16];
	for (int i = 0; i < 16; i++)
	{
		ln1[i] = ' ';
		ln2[i] = ' ';
	}

	while (true)
	{
		ChassisSet(joystickGetAnalog(1,3), joystickGetAnalog(1,2), false);
		//JoystickControl();
		
		
		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127, false);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-127, false);
		else LiftSet(0, false);
		

		//if (buttonIsNewPress(JOY1_6U)) LiftSetHeight(1000);
		//if (buttonIsNewPress(JOY1_6D)) LiftSetHeight(400);

		//if (!joystickGetDigital(1, 8, JOY_UP)) LiftContinuous();

		if (buttonIsNewPress(JOY1_7U)) ScoringMechSwitch();
		
		snprintf(ln1, 16, "L: %+05d", LiftGetCalibratedPotentiometerLeft());
		snprintf(ln2, 16, "R: %+05d", LiftGetCalibratedPotentiometerRight());

		printText(ln1, Centered, 1);
		printText(ln2, Centered, 2);
		
		delay(100);
	}
}