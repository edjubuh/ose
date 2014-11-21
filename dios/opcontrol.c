/**
 * @file opcontrol.c
 * @brief Source file for operator control.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ************************************************************************/

#include "main.h"
#include "dios/Chassis.h"
#include "dios/Lift.h"
#include "dios/CortexDefinitions.h"
#include <math.h>

/**
 * @brief Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	int left = 0, right = 0;
	while (true)
	{
		ChassisSet(joystickGetAnalog(1,3), joystickGetAnalog(1,2), false);
		
		imeGet(I2C_MOTOR_LIFT_LEFT, &left);
		imeGet(I2C_MOTOR_LIFT_RIGHT, &right);

		lcdPrint(uart1, 1, "left: %d", left);
		lcdPrint(uart1, 2, "right: %d", right);

		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSetHeight(100);
		else LiftSetHeight(0);
		
		/*if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127, false);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-127, false);
		else
			LiftSet(0, false);*/
		
		delay(20);
	}
}