/************************************************************************/
/* @file MasterSlavePIDController.h										*/
/* @brief Header file for master/slave PID controller functions			*/
/* See libsml/MasterSlavePIDController.c for details of all functions   */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef MASTER_SLAVE_PID_CONTROLLER_H_
#define MASTER_SLAVE_PID_CONTROLLER_H_

#include "main.h"
#include "sml/SmartMotorLibrary.h"

typedef struct
{
	PIDController master;
	PIDController slave;
	Mutex mutex;
} MasterSlavePIDController;

MasterSlavePIDController CreateMasterSlavePIDController(PIDController master, PIDController slave);

void InitializeMasterSlaveController(MasterSlavePIDController *controller, int masterGoal);

void MasterSlavePIDControllerTask(MasterSlavePIDController *controller);

#endif