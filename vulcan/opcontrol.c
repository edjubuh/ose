/**
 * @file vulcan/opcontrol.c
 * @author Elliot Berman and Robert Shrote
 * @brief Source file for operator control.
 *
 * @copyright Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/main.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

//#include "sml/recorder.h"
#include "vulcan/buttons.h"
#include "vulcan/mechop.h"
#include "vulcan/CortexDefinitions.h"
#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"
#include "vulcan/LCDDisplays.h"

//#include "sml/recorder.h"

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
	//fdelete("trial.cvs");
	//start = millis();
	//recorderUser specific commands above
	while (true)
	{
		if (buttonIsNewPress(JOY1_8D)) mode = !mode;

		// Tank Control
		//ChassisSet((mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), false); 

		// Mecanum Control
		JoystickControl((mode ? -joystickGetAnalog(1, 4) : joystickGetAnalog(1, 1)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), (mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 1) : joystickGetAnalog(1, 4)));

		// Recorded Control
		//recorderUser((mode ? -joystickGetAnalog(1, 4) : joystickGetAnalog(1, 1)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), (mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 1) : joystickGetAnalog(1, 4)));

		/*
		if (joystickGetDigital(1, 6, JOY_UP))
		LiftSet(127, false);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
		LiftSet(-127, false);
		else LiftSet(0, false);
		*/
		
		if (mode && buttonIsNewPress(JOY1_8U))
		{
			LiftSetHeight(80);
			pidEnabled = true;
		}
		if (!mode && buttonIsNewPress(JOY1_8U))
		{
			LiftSetHeight(40);
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
			LiftSet(-100, false);
			pidEnabled = false;
		}
		else if (!pidEnabled)
			LiftSet(0, false);

		ScoringMechNeedleSet(!(joystickGetDigital(1, 7, JOY_UP) || joystickGetDigital(1,5,JOY_DOWN)));

		if (buttonIsNewPress(JOY1_7D) || buttonIsNewPress(JOY1_5U))
			ScoringMechClawSwitch();

		//snprintf(ln1, 16, "L:%+05d;R:%+05d", LiftGetCalibratedPotentiometerLeft(), LiftGetCalibratedPotentiometerRight());
		

		//lcdprint(Centered, 1, "Vulcan aae5f23");

		//lcdprintf(Centered, 1, "pl:%04d r:%04d", LiftGetCalibratedPotentiometerLeft(), LiftGetCalibratedPotentiometerRight());
		lcdprintf(Centered, 2, "el:%02d r:%02d", LiftGetQuadEncLeft(), LiftGetQuadEncRight());

		delay(100);
	}
}