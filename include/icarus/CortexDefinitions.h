/************************************************************************/
/* @file CortexDefinitions.h											*/
/* @brief Header file containing Cortex wiring constants                */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef CORTEX_DEFINITIONS_H_
#define CORTEX_DEFINITIONS_H_

#include "main.h"

/* --- MOTOR DEFINITIONS --- */

#define MOTOR_CHASSIS_FRONTRIGHT		4
#define MOTOR_CHASSIS_FRONTRIGHTinv		1

#define MOTOR_CHASSIS_FRONTLEFT			3
#define MOTOR_CHASSIS_FRONTLEFTinv		-1

#define MOTOR_CHASSIS_REARRIGHT			2
#define MOTOR_CHASSIS_REARRIGHTinv		1

#define MOTOR_CHASSIS_REARLEFT			5
#define MOTOR_CHASSIS_REARLEFTinv		1

#define MOTOR_LIFT_TOPLEFT				1
#define MOTOR_LIFT_TOPLEFTinv			-1

#define MOTOR_LIFT_TOPRIGHT				6
#define MOTOR_LIFT_TOPRIGHTinv			1

#define MOTOR_LIFT_BOTLEFT				7
#define MOTOR_LIFT_BOTLEFTinv			1

#define MOTOR_LIFT_BOTRIGHT				8
#define MOTOR_LIFT_BOTRIGHTinv			1

#define MOTOR_LIFT_MIDLEFT				9
#define MOTOR_LIFT_MIDLEFTinv			1

#define MOTOR_LIFT_MIDRIGHT				10
#define MOTOR_LIFT_MIDRIGHTinv			1


/* --- DIGITAL DEFINITIONS --- */

#define DIG_LIFT_BOTLIM					1
#define DIG_SCORINGMECH					10


/* --- ANALOG DEFINITIONS --- */

#define ANA_GYROSCOPE					1
#define ANA_POWEREXP					2


/* --- UART DEFINITIONS --- */

#define UART_LCDDISPLAY					uart1


/* --- I2C DEFINITIONS --- */

#define I2C_MOTOR_CHASSIS_FRONTRIGHT	1
#define I2C_MOTOR_CHASSIS_FRONTLEFT		2
#define I2C_MOTOR_CHASSIS_REARRIGHT		3
#define I2C_MOTOR_CHASSIS_REARLEFT		4
#define I2C_MOTOR_LIFT					5
#define I2C_MOTOR_SCORINGMECH			6

#endif