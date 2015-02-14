/**
 * @file vulcan/opcontrol.c
 * @brief Source file for operator control.
 *
 * @copyright Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/main.h for additional notice.
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
#include "vulcan/LCDDisplays.h"

bool mode = false; // true: skyrise, false: cubes
bool pidEnabled = false;

/**
 * Returns a string representing the current robot state
 */
/*
char * getRobotState()
{
	if (!isAutonomous()) return;
	if (pidEnabled && mode)
		return "   PID | Skyrise";
	if (pidEnabled && !mode)
		return "   PID | Cube   ";
	if (!pidEnabled && mode)
		return " NoPID | Skyrise";
	if (!pidEnabled && !mode)
		return " NoPID | Cube   ";
}
*/

/**
 * Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	while (true)
	{
		if (buttonIsNewPress(JOY1_8D)) mode = !mode;

		//ChassisSet((mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), false);
		JoystickControl();

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

		if (buttonIsNewPress(JOY1_8R))
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

		ScoringMechNeedleSet(!joystickGetDigital(1, 7, JOY_UP));

		ScoringMechClawSet(!joystickGetDigital(1, 7, JOY_DOWN));

		//snprintf(ln1, 16, "L:%+05d;R:%+05d", LiftGetCalibratedPotentiometerLeft(), LiftGetCalibratedPotentiometerRight());
		

		//lcdprint(Centered, 1, "Vulcan aae5f23");

		delay(100);
	}
}