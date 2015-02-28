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
#include "sml/SmartMotorLibrary.h"

#define MOTOR_CHASSIS_REARRIGHT			1
#define MOTOR_LIFT_MIDDLELEFT			2
#define MOTOR_LIFT_FRONTRIGHT			3
#define MOTOR_LIFT_FRONTLEFT			4
#define MOTOR_LIFT_MIDDLERIGHT			5
#define MOTOR_CHASSIS_FRONTRIGHT		6
#define MOTOR_CHASSIS_FRONTLEFT			7
#define MOTOR_LIFT_REARRIGHT			8
#define MOTOR_LIFT_REARLEFT				9
#define MOTOR_CHASSIS_REARLEFT			10

void LiftSet(int in)
{
	MotorSet(MOTOR_LIFT_MIDDLELEFT, in, false);
	MotorSet(MOTOR_LIFT_FRONTRIGHT, in, false);
	MotorSet(MOTOR_LIFT_REARRIGHT, in, false);
	MotorSet(MOTOR_LIFT_FRONTLEFT, in, false);
	MotorSet(MOTOR_LIFT_MIDDLERIGHT, in, false);
	MotorSet(MOTOR_LIFT_REARLEFT, in, false);
}

/**
* @brief Sets motors in motion based on user input (from controls).
*/
void operatorControl() 
{
	InitializeMotorManager();
	MotorConfigure(MOTOR_LIFT_FRONTLEFT, true, 127);
	MotorConfigure(MOTOR_LIFT_FRONTRIGHT, false, 127);
	MotorConfigure(MOTOR_LIFT_MIDDLELEFT, true, 127);
	MotorConfigure(MOTOR_LIFT_MIDDLERIGHT, false, 127);
	MotorConfigure(MOTOR_LIFT_REARLEFT, false, 127);
	MotorConfigure(MOTOR_LIFT_REARRIGHT, false, 127);
	while (true)
	{
		if (joystickGetDigital(1, 6, JOY_UP))
			LiftSet(127);
		else if (joystickGetDigital(1, 6, JOY_DOWN))
			LiftSet(-100);
		else
			LiftSet(0);
		delay(25);
	}
}
