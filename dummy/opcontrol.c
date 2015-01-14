/**
 * @file dummy/opcontrol.c
 * @brief Source file for dummy operator functions
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
			motorSet(5, 127);
		delay(1000);
			motorSet(5, -127);
		delay(1000);
			motorSet(5, 0);
		delay(2000);
	}
}
