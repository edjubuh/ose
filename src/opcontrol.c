/**
 * @file src/opcontrol.c		
 * @brief Source file for operator control.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "lcd/LCDFunctions.h"

/**
* @brief Sets motors in motion based on user input (from controls).
*/
void operatorControl() 
{
	lcdInitialize();
	while (true)
	{
		lcdprintf(Centered, 1, "%d", (analogRead(1)/10));
		lcdprintf(Centered, 2, "%d", (analogRead(2)/10));

		delay(20);
	}
}
