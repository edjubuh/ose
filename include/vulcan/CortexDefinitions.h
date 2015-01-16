/**
* @file include/vulcan/CortexDefinitions.h
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
// 6: A		7: B		8: C		9: D
#define MOTOR_CHASSIS_REARRIGHT			1
#define MOTOR_LIFT_MIDDLERIGHT			2
#define MOTOR_LIFT_MIDDLELEFT			3
#define MOTOR_LIFT_FRONTLEFT			4
#define MOTOR_LIFT_FRONTRIGHT			5
#define MOTOR_CHASSIS_FRONTRIGHT		6
#define MOTOR_CHASSIS_FRONTLEFT			7
#define MOTOR_LIFT_REARRIGHT			8
#define MOTOR_LIFT_REARLEFT				9
#define MOTOR_CHASSIS_REARLEFT			10

/* --- DIGITAL DEFINITIONS --- */

#define DIG_SCORINGMECH					1
#define DIG_LIFT_BOTLIM_LEFT			2
#define DIG_LIFT_BOTLIM_RIGHT			3


/* --- ANALOG DEFINITIONS --- */

#define ANA_POT_LIFT_LEFT				2
#define ANA_POT_LIFT_RIGHT				3
#define ANA_POWEREXP					1
#define ANA_GYROSCOPE					4


/* --- UART DEFINITIONS --- */

#define UART_LCDDISPLAY					uart1


/* --- I2C DEFINITIONS --- */

#define I2C_MOTOR_CHASSIS_RIGHT			1
#define I2C_MOTOR_CHASSIS_LEFT			2

#endif