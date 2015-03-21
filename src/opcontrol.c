/**
 * @file src/opcontrol.c		
 * @brief Source file for operator control.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"


/**
* @brief Sets motors in motion based on user input (from controls).
*/
void operatorControl() 
{
	while (true)
	{
		motorSet(2, joystickGetAnalog(1, 3));
		motorSet(3, joystickGetAnalog(1, 2));
		if (joystickGetDigital(1, 6, JOY_UP))
			motorSet(4, 127);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			motorSet(4, -127);
		else
			motorSet(4, 0);
		delay(25);
	}
}
