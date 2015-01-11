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
		motorSet(2, joystickGetAnalog(1, 2));
		motorSet(3, joystickGetAnalog(1, 2));
		motorSet(4, joystickGetAnalog(1, 2));
		motorSet(7, joystickGetAnalog(1, 3));
		motorSet(8, joystickGetAnalog(1, 3));
		motorSet(9, joystickGetAnalog(1, 3));

		delay(20);
	}
}
