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
#include "sml/SmartMotorLibrary.h"
#include <math.h>

/**
 * @brief Sets motors in motion based on user input (from controls).
 */
void operatorControl()
{
	Gyro gyro = gyroInit(5, 0);
	while (true)
	{
		//lcdPrint(uart1, 1, "l: %d, r: %d", digitalRead(DIG_LIFT_BOTLIM_LEFT), digitalRead(DIG_LIFT_BOTLIM_RIGHT));
		//lcdPrint(uart1, 1, "l: %+3d, r: %+3d", MotorGet(MOTOR_LIFT_REARLEFT), -MotorGet(MOTOR_LIFT_REARRIGHT));
		//lcdPrint(uart1, 2, "l: %4d, r: %4d", LiftGetCalibratedPotentiometerLeft(), LiftGetCalibratedPotentiometerRight());
	
		ChassisSet(joystickGetAnalog(1,3), joystickGetAnalog(1,2), false);
		
		
		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(10);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-10);
		else
			LiftSet(0);
		
		delay(20);
	}
}