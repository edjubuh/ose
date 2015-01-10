/********************************************************************************/
/* @file dummy/init.c		@brief Source file for dummy operator functions.	*/
/*																				*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.			*/
/* Portions of this file may contain elements from the PROS API.				*/
/* See include/API.h for additional notice.										*/
/********************************************************************************/

#include "main.h"
#include "dummy/MotorDefinitions.h"
#include "sml/SmartMotorLibrary.h"

void operatorControl() 
{
	for (int i = 1; i <= 12; i++)
		pinMode(i, OUTPUT);

	while (true) 
	{
		for (int i = 1; i < 11; i++)
			motorSet(i, joystickGetAnalog(1, 2));

		for (int i = 1; i <= 12; i++)
			digitalWrite(i, joystickGetDigital(1, 7, JOY_UP));

		delay(20);
	}
}
