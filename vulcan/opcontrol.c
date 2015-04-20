/**
 * @file vulcan/opcontrol.c
 * @author Elliot Berman and Robert Shrote
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

#define NEEDLE_DEPLOY_DURATION			2000

bool pidEnabled = false;


/**
* @brief Control schema for Josh's driving preferences
*/
void JoshControl()
{
	long startNeedleDeploy = -NEEDLE_DEPLOY_DURATION;
	bool mode = true; // true is standard "forward is forward" control and false is "forward is backward" control for use when building skyrise
	while (true)
	{
		// ---------- VARIOUS SWITCHES ---------- //
#ifdef AUTO_DEBUG
		if (buttonIsNewPress(JOY1_7L)) autonomous();
#endif
		if (buttonIsNewPress(JOY1_7D)) mode = !mode;

		// ---------- CHASSIS CONTROL ---------- //
		// Tank Control
		//ChassisSet((mode ? -joystickGetAnalog(1, 2) : joystickGetAnalog(1, 3)), (mode ? -joystickGetAnalog(1, 3) : joystickGetAnalog(1, 2)), false); 

		// Mecanum Control
		JoystickControl(mode ? joystickGetAnalog(1, 1) : -joystickGetAnalog(1, 4), joystickGetAnalog(1, 2),
			joystickGetAnalog(1, 3), mode ? joystickGetAnalog(1, 4) : -joystickGetAnalog(1, 1));

		// ------------ LIFT CONTROL ------------ //
		if (buttonIsNewPress(JOY1_8U))
		{ // High Post
			LiftSetHeight(85);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_8R))
		{ // Medium Post 
			LiftSetHeight(13);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_8L))
		{ // Low Post
			LiftSetHeight(20);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_7U))
		{ // Single cube
			LiftSetHeight(8);
			pidEnabled = true;
		}
		else if (buttonIsNewPress(JOY1_8D))
		{ // Ground
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

		// --------- SCORE MECH CONTROL --------- //
		if (joystickGetDigital(1, 5, JOY_DOWN))
			startNeedleDeploy = millis();

		if (millis() - startNeedleDeploy > NEEDLE_DEPLOY_DURATION)
			ScoringMechNeedleSet(true);
		else
			ScoringMechNeedleSet(false);

		if (buttonIsNewPress(JOY1_5U))
		{

			if (LiftGetQuadEncLeft() < 5 && !ScoringMechClawGet())
			{
				switch (skyriseBuilt)
				{
				case 0:
					LiftSetHeight(15);
					break;
				case 1:
					LiftSetHeight(18);
					break;
				case 2:
					LiftSetHeight(33);
					break;
				case 3:
					LiftSetHeight(48);
					break;
				case 4:
					LiftSetHeight(67);
					break;
				case 5:
					LiftSetHeight(83);
					break;
				case 6:
					LiftSetHeight(150);
					break;
				default:
					LiftSetHeight(13);
				}
				skyriseBuilt++;
				pidEnabled = true;
			}

			ScoringMechClawSwitch();
		}

		// ----------- DRIVER SWITCH ----------- //
		if (buttonIsNewPress(JOY1_7R))
			SamControl();

		// ------------ LCD PRINTERS ----------- //
		lcdprintf(Centered, 1, "J Vulcan %s", VERSION);
		lcdprint(Centered, 2, "teleop");
		//lcdprintf(Centered, 2, "cl:%04d r:%04d", ChassisGetIMELeft(), ChassisGetIMERight());
		//lcdprintf(Centered, 2, "el:%02d r:%02d", LiftGetQuadEncLeft(), LiftGetQuadEncRight());
		//lcdprintf(Centered, 2, "il:%04d r: %04d", ChassisGetIRRight(), ChassisGetIRLeft());
		//lcdprintf(Centered, 2, "l:%d r:%d", ChassisHasIRLineLeft(Grey), ChassisHasIRLineRight(Grey));
		/*int ir = ChassisGetIRRight();
		lcdprintf(Centered, 1, "g:%d  v:%04d", (ir < 600) ? 1 : 0, ir);
		lcdprintf(Centered, 2, "r:%d  b:%d", (ir < 450) ? 1 : 0, (ir < 300) ? 1 : 0);*/

		delay(15);
	}
}

/**
* @brief Control schema for Sam's driving preferences
*/
void SamControl()
{
	long startNeedleDeploy = -NEEDLE_DEPLOY_DURATION;
	while (true)
	{
#ifdef AUTO_DEBUG
		if (buttonIsNewPress(JOY1_7L)) autonomous();
#endif

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
		{ // High post
			LiftSetHeight(90);
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
		if (joystickGetDigital(1, 5, JOY_DOWN))
			startNeedleDeploy = millis();

		if (millis() - startNeedleDeploy > NEEDLE_DEPLOY_DURATION)
			ScoringMechNeedleSet(true);
		else
			ScoringMechNeedleSet(false);

		if (buttonIsNewPress(JOY1_5U))
		{
			// If lift is on ground and we're grabbing a skyrise, automatically go up above the autoloader
			if (LiftGetQuadEncLeft() < 5 && !ScoringMechClawGet())
			{
				LiftSetHeight(15);
				pidEnabled = true;
			}

			ScoringMechClawSwitch();
		}

		// ----------- DRIVER SWITCH ----------- //
		if (buttonIsNewPress(JOY1_7D))
			JoshControl();

		// ------------ LCD PRINTERS ----------- //
		lcdprintf(Centered, 1, "S Vulcan %s", VERSION);
		lcdprint(Centered, 2, "opcontrol");
		//lcdprintf(Centered, 1, "cl:%04d r:%04d", ChassisGetIMELeft(), ChassisGetIMERight());
		//lcdprintf(Centered, 2, "el:%02d r:%02d", LiftGetQuadEncLeft(), LiftGetQuadEncRight());
		//lcdprintf(Centered, 2, "il:%04d r: %04d", ChassisGetIRRight(), ChassisGetIRLeft());
		//lcdprintf(Centered, 2, "l:%d/%04d r:%d/%04d", ChassisHasIRLineLeft(Grey), ChassisGetIRLeft(), ChassisHasIRLineRight(Grey), ChassisGetIRLeft());
		/*int ir = ChassisGetIRRight();
		lcdprintf(Centered, 1, "g:%d  v:%04d", (ir < 600) ? 1 : 0, ir);
		lcdprintf(Centered, 2, "r:%d  b:%d", (ir < 450) ? 1 : 0, (ir < 300) ? 1 : 0);*/

		delay(15);
	}
}

/**
 * @brief Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	if (digitalRead(DIG_DRIVER_JUMPER)) // Jumper out: Josh is driver
		JoshControl();
	else
		SamControl();
}