/**
 * @file vulcan/opcontrol.c
 * @author Elliot Berman and Robert Shrote
 * @brief Source file for operator control.
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
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


bool pidEnabled = false;

/**
 * @brief Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	while (true)
	{
		if (buttonIsNewPress(JOY1_7L)) autonomous();

		// ---------- CHASSIS CONTROL ---------- //
		// Tank Control
		//ChassisSet((mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), false); 

		// Mecanum Control
		JoystickControl(joystickGetAnalog(1, 1), joystickGetAnalog(1, 2), joystickGetAnalog(1, 3), joystickGetAnalog(1, 4));

		// ------------ LIFT CONTROL ------------ //
		if (buttonIsNewPress(JOY1_8U))
		{ // Ground
			LiftSetHeight(0);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_8R))
		{ // Medium Post 
			LiftSetHeight(35);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_8L))
		{ // Low Post
			LiftSetHeight(20);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_8D))
		{ // High Post
			LiftSetHeight(85);
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

		// --------- SCORE MECH CONTROL --------- //
		ScoringMechNeedleSet(!(joystickGetDigital(1, 7, JOY_UP) || joystickGetDigital(1,5,JOY_DOWN)));

		if (buttonIsNewPress(JOY1_7D) || buttonIsNewPress(JOY1_5U))
			ScoringMechClawSwitch();
		
		// ------------ LCD PRINTERS ----------- //
		//lcdprint(Centered, 1, "Vulcan aae5f23");

		lcdprintf(Centered, 1, "cl:%04d r:%04d", ChassisGetIMELeft(), ChassisGetIMERight());
		lcdprintf(Centered, 2, "el:%02d r:%02d", LiftGetQuadEncLeft(), LiftGetQuadEncRight());
		//lcdprintf(Centered, 2, "il:%04d r: %04d", ChassisHasIRLineRight(Grey), ChassisHasIRLineLeft(Grey));
		/*int ir = ChassisGetIRRight();
		lcdprintf(Centered, 1, "g:%d  v:%04d", (ir < 600) ? 1 : 0, ir);
		lcdprintf(Centered, 2, "r:%d  b:%d", (ir < 450) ? 1 : 0, (ir < 300) ? 1 : 0);*/

		delay(15);
	}
}