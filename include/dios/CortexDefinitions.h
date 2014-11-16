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

#define MOTOR_CHASSIS_FRONTRIGHT		8
#define MOTOR_CHASSIS_FRONTLEFT			7
#define MOTOR_CHASSIS_REARRIGHT			9
#define MOTOR_CHASSIS_REARLEFT			10
#define MOTOR_LIFT_TOPLEFT				2
#define MOTOR_LIFT_TOPRIGHT				1
#define MOTOR_SCORINGMECH				3


/* --- DIGITAL DEFINITIONS --- */

#define DIG_LIFT_BOTLIM					1
#define DIG_SCORINGMECH					2


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