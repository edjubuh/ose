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
#include "sml/MasterSlavePIDController.h"
#include "sml/SmartMotorLibrary.h"
#include <main.h>

/**
* Creates a MasterSlavePIDController struct based off of the parameters.
* @param PIDController master
*			The controller for the master process. The call/execute methods should only affect the master. (i.e. the execute should not set the output for both slave and master)
* @param PIDController slave
*			The controller for the slave process.
*/
MasterSlavePIDController CreateMasterSlavePIDController(PIDController master, PIDController slave, PIDController equalizer, bool enabledPrimaryPID)
{
	MasterSlavePIDController controller;
	controller.master = master;
	controller.slave = slave;
	controller.equalizer = equalizer;
	controller.enabledPrimaryPID = enabledPrimaryPID;
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
	controller->manualPrimaryOutput = 0;
	mutexGive(controller->mutex);
	return taskCreate(MasterSlavePIDControllerTask, TASK_DEFAULT_STACK_SIZE, controller, TASK_PRIORITY_DEFAULT);
}

void MasterSlavePIDControllerTask(void *c)
{
	MasterSlavePIDController *controller = c;
	PIDController *master = &controller->master;
	PIDController *slave = &controller->slave;
	PIDController *equalizer = &controller->equalizer;
	int masterOutput, slaveOutput;
	while(true)
	{
		// Can't take mutex this round. Skip loop and try again.
		if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT)) continue;

		masterOutput = controller->enabledPrimaryPID ? PIDControllerCompute(master) : controller->manualPrimaryOutput;
		slaveOutput = controller->enabledPrimaryPID ? PIDControllerCompute(slave) : controller->manualPrimaryOutput;
		
		slaveOutput += PIDControllerCompute(equalizer);
		
		int max = 127;
		if(abs(masterOutput) > max)
			max = abs(masterOutput);
		if(abs(slaveOutput) > max)
			max = abs(slaveOutput);
		
		double scale = 127.0 / max;
		
		masterOutput = (int)(masterOutput * scale);
		slaveOutput  = (int)(slaveOutput * scale);

		//lcdPrint(uart1, 1, "m: %+3d, s: %+3d", masterOutput, slaveOutput);
		lcdPrint(uart1, 2, "m: %4d, s: %4d", master->Call(), slave->Call());
		
		master->Execute(masterOutput, false);
		slave->Execute(slaveOutput, false);
		
		mutexGive(controller->mutex);

		delay(40);
	}
}

void MasterSlavePIDChangeGoal(MasterSlavePIDController *controller, int primaryPIDGoal)
{
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
	
	controller->enabledPrimaryPID = true;
	
	controller->master.Goal = primaryPIDGoal;
	controller->master.Goal = primaryPIDGoal;
	
	mutexGive(controller->mutex);
}

void MasterSlavePIDIncreaseGoal(MasterSlavePIDController *controller, int deltaGoal)
{
	if (!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;

	if (!controller->enabledPrimaryPID)
	{
		controller->enabledPrimaryPID = true;
		controller->master.Goal = controller->master.Call();
		controller->slave.Goal = controller->master.Call();
	}

	controller->master.Goal += deltaGoal;
	controller->slave.Goal += deltaGoal;

	//static int c = 0;
	//lcdPrint(uart1, 1, "C: %d", c++);
	//lcdPrint(uart1, 2, "V: %d", controller->slave.Goal);

	mutexGive(controller->mutex);
}

void MasterSlavePIDSetOutput(MasterSlavePIDController *controller, int output)
{
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
		
	controller->enabledPrimaryPID = false;
	
	controller->manualPrimaryOutput = output;
	
	mutexGive(controller->mutex);
}


static void VivaLaRevolucion(MasterSlavePIDController *controller)
{
	// Judges, please talk to us about VIVA LA REVOLUCION!
	PIDController temp = controller->master;
	controller->master = controller->slave;
	controller->slave = temp;
}

