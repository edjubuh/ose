/************************************************************************/
/* @file MasterSlavePIDController.c										*/
/* @brief Extension of SingleThreadPIDController to implement 			*/
/*		  a multitasked Master/Slave PID Controller						*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "sml/SingleThreadPIDController.h"
#include "sml/MasterSlavePIDContoller.h"
#include "sml/SmartMotorLibrary.h"

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
TaskHandle InitializeMasterSlaveController(MasterSlavePIDController *controller, int masterGoal)
{
	controller->mutex = mutexCreate();
	mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT);
	controller->slave.Goal = 0;
	controller->master.Goal = masterGoal;
	mutexGive(controller->mutex);
	return taskCreate(MasterSlavePIDControllerTask, TASK_DEFAULT_STACK_SIZE, controller, TASK_PRIORITY_DEFAULT);
}

void MasterSlavePIDControllerTask(void *c)
{
	MasterSlavePIDController *controller = c;
	PIDController *master = &controller->master;
	PIDController *slave = &controller->slave;
	int masterOutput, slaveOutput;
	while(true)
	{
		// Can't take mutex this round. Skip loop and try again.
		if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT)) continue;
		
		masterOutput = PIDControllerCompute(master);
		
		int slaveErr = master->Call() - slave->Call();
		
		slave->integral += slaveErr;
		
		if(slave->integral < slave->MinIntegral)
			slave->integral = slave->MinIntegral;
		else if(slave->integral > slave->MaxIntegral)
			slave->integral = slave->MaxIntegral;
			
		long derivative = (slaveErr - slave->prevError) / ((micros() - slave->prevTime) * 1000000);
		
		slaveOutput = (int)((slave->Kp * slaveErr) + (slave->Ki * slave->integral) +  (slave->Kd * derivative));
		
		slave->prevTime = micros();
		slave->prevError = slaveErr;
		
		slave->Execute(masterOutput + slaveOutput, false);
		master->Execute(masterOutput - slaveOutput, false);
		
		delay(20);
	}
}

void MasterSlavePIDChangeGoal(MasterSlavePIDController *controller, int masterGoal)
{
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
	
	controller->master.Goal = masterGoal;
	
	mutexGive(controller->mutex);
}