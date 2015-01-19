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
#include "vulcan/mechop.h"
#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"


/**
 * Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	bool mode = false; // true: skyrise, false: cubes
	bool pidEnabled = false;
	int displayRound = 0;

	while (true)
	{
		if (buttonIsNewPress(JOY1_8D)) mode = !mode;

		ChassisSet((mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), false);
		//JoystickControl();

		/*
		if (joystickGetDigital(1, 6, JOY_UP))
		LiftSet(127, false);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
		LiftSet(-127, false);
		else LiftSet(0, false);
		*/

		if (mode && buttonIsNewPress(JOY1_8U))
		{
			LiftSetHeight(125);
			pidEnabled = true;
		}
		if (!mode && buttonIsNewPress(JOY1_8U))
		{
			LiftSetHeight(350);
			pidEnabled = true;
		}

		if (buttonIsNewPress(JOY1_7D))
		{
			LiftSetHeight(0);
			pidEnabled = true;
		}

		if (joystickGetDigital(1, 6, JOY_UP))
		{
			LiftSet(127, false);
			pidEnabled = false;
		}
		else if (joystickGetDigital(1, 6, JOY_DOWN))
		{
			LiftSet(-90, false);
			pidEnabled = false;
		}
		else if (!pidEnabled)
			LiftSet(0, false);

		if(pidEnabled) LiftContinuous();

		ScoringMechSet(!joystickGetDigital(1, 7, JOY_UP));

		//snprintf(ln1, 16, "L:%+05d;R:%+05d", LiftGetCalibratedPotentiometerLeft(), LiftGetCalibratedPotentiometerRight());
		if (pidEnabled && mode)
			printText("   PID | Skyrise", Left, 2);
		if (pidEnabled && !mode)
			printText("   PID | Cube", Left, 2);
		if (!pidEnabled && mode)
			printText(" NoPID | Skyrise", Left, 2);
		if (!pidEnabled && !mode)
			printText(" NoPID | Cube", Left, 2);

		printText("Vulcan aae5f23", Centered, 1);

		delay(100);
	}
}