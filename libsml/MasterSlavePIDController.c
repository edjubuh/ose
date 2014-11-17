/************************************************************************/
/* @file MasterSlavePIDController.c										*/
/* @brief Extension of SingleThreadPIDController to implement 			*/
/*		  a multitasked Master/Slave PID Controller						*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
//#include "sml/"
#include "sml/MasterSlavePIDContoller.h"

/**
* Creates a MasterSlavePIDController struct based off of the parameters.
* @param PIDController master
*			The controller for the master process. The call/execute methods should only affect the master. (i.e. the execute should not set the output for both slave and master)
* @param PIDController slave
*			The controller for the slave process.
*/
MasterSlavePIDController CreateMasterSlavePIDController(PIDController master, PIDController slave)
{
	MasterSlavePIDController controller;
	controller.master = master;
	controller.slave = slave;
	return controller;
}

/**
* Initializes the 
*/
void InitializeMasterSlaveController(MasterSlavePIDController *controller, int masterGoal)
{
	controller->mutex = mutexCreate();
	mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT);
	controller->slave.Goal = 0;
	controller->master.Goal = masterGoal;
	mutexGive(controller->mutex);
	taskCreate(MasterSlavePIDControllerTask, TASK_DEFAULT_TASK_SIZE, controller, TASK_PRIORITY_DEFAULT);
}

void MasterSlavePIDControllerTask(MasterSlavePIDController *controller)
{
	while(true)
	{
		// Can't take mutex this round. Skip loop and try again.
		if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT)) continue;
		
		
		
		delay(20);
	}
}