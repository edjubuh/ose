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
	bool enabledMasterPID;
	int manualMasterOutput;
	bool enabled;
} MasterSlavePIDController;

MasterSlavePIDController CreateMasterSlavePIDController(PIDController master, PIDController slave, bool enabledMasterPID, bool enabled);

TaskHandle InitializeMasterSlaveController(MasterSlavePIDController *controller, int masterGoal);

void MasterSlavePIDControllerTask(void *controller);

void MasterSlavePIDChangeGoal(MasterSlavePIDController *controller, int masterGoal);

void MasterSlavePIDSetOutput(MasterSlavePIDController *controller, int output);

void MasterSlavePIDSetEnabled(MasterSlavePIDController *controller, bool enabled);

#endif