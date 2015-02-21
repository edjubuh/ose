/**
 * @file dummy/opcontrol.c
 * @brief Source file for dummy operator functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "lcd/LCDManager.h"
#include "lcd/LCDFunctions.h"

//630MulberryStreet46077

/**
* @brief Sets motors in motion based on user input (from controls).
*/
void operatorControl() 
{
	Encoder enc = encoderInit(1, 2, false);
	Encoder enc0 = encoderInit(3, 4, false);
	
	while (true) 
	{
		lcdprintf(Centered, 1, "%d", encoderGet(enc));
		lcdprintf(Centered, 2, "%d", encoderGet(enc0));

		if (lcdReadButtons(uart1) != 0)
		{
			encoderReset(enc);
			encoderReset(enc0);
		}
		delay(150);
	}
}
