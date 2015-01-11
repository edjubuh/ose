/**
 * @file include/dios/CortexDefinitions.h
 * @brief Header file containing Cortex wiring constants
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef CORTEX_DEFINITIONS_H_
#define CORTEX_DEFINITIONS_H_

#include "main.h"

/* --- MOTOR DEFINITIONS --- */

#define MOTOR_LIFT_REARRIGHT			1
#define MOTOR_CHASSIS_REARLEFT			2
#define MOTOR_CHASSIS_FRONTLEFT			3
#define MOTOR_CHASSIS_FRONTRIGHT		4
#define MOTOR_CHASSIS_REARRIGHT			5
#define MOTOR_LIFT_MIDDLELEFT			6
#define MOTOR_LIFT_MIDDLERIGHT			7
#define MOTOR_LIFT_FRONTLEFT			8
#define MOTOR_LIFT_FRONTRIGHT			9
#define MOTOR_LIFT_REARLEFT				10

/* --- DIGITAL DEFINITIONS --- */

#define DIG_LIFT_BOTLIM_LEFT			1
#define DIG_LIFT_BOTLIM_RIGHT			2


/* --- ANALOG DEFINITIONS --- */

#define ANA_GYROSCOPE					1
#define ANA_POWEREXP					2
#define ANA_POT_LIFT_LEFT				3
#define	ANA_POT_LIFT_RIGHT				4


/* --- UART DEFINITIONS --- */

#define UART_LCDDISPLAY					uart1


/* --- I2C DEFINITIONS --- */

#define I2C_MOTOR_CHASSIS_FRONTRIGHT	0
#define I2C_MOTOR_CHASSIS_FRONTLEFT		1
#define I2C_MOTOR_CHASSIS_REARRIGHT		2
#define I2C_MOTOR_CHASSIS_REARLEFT		3
#define I2C_MOTOR_LIFT_LEFT				4
#define I2C_MOTOR_LIFT_RIGHT			5

#endif