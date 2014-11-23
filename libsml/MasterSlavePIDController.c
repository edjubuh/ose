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
MasterSlavePIDController CreateMasterSlavePIDController(PIDController master, PIDController slave, bool enabledMasterPID)
{
	MasterSlavePIDController controller;
	controller.master = master;
	controller.slave = slave;
	controller.enabledMasterPID = enabledMasterPID;
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
	controller->manualMasterOutput = 0;
	mutexGive(controller->mutex);
	return taskCreate(MasterSlavePIDControllerTask, TASK_DEFAULT_STACK_SIZE, controller, TASK_PRIORITY_DEFAULT);
}

void MasterSlavePIDControllerTask(void *c)
{
	MasterSlavePIDController *controller = c;
	PIDController *master = &controller->master;
	PIDController *slave = &controller->slave;
	int masterOutput, slaveOutput, masterExecute, slaveExecute;
	while(true)
	{
		// Can't take mutex this round. Skip loop and try again.
		if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT)) continue;
		
		masterOutput = controller->enabledMasterPID ? PIDControllerCompute(master) : controller->manualMasterOutput;
		
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
		
		masterExecute = masterOutput - (slaveOutput/2);
		slaveExecute = masterOutput + (slaveOutput/2);

		int max = 127;
		if(abs(masterExecute) > max)
			max = abs(masterExecute);
		if(abs(slaveExecute) > max)
			max = abs(slaveExecute);
		
		double scale = 127.0 / max;
		
		masterExecute = (int)(masterExecute * scale);
		slaveExecute  = (int)(slaveExecute * scale);
		
		
		//lcdPrint(uart1, 1, "m: %d, s: %d", masterExecute, slaveExecute);
		//lcdPrint(uart1, 2, "m: %d, s: %d", master->Call(), slave->Call());
		
		master->Execute(masterExecute, false);
		slave->Execute(slaveExecute, false);
		
		mutexGive(controller->mutex);

		delay(40);
	}
}

void MasterSlavePIDChangeGoal(MasterSlavePIDController *controller, int masterGoal)
{
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
	
	controller->enabledMasterPID = true;
	
	controller->master.Goal = masterGoal;
	
	mutexGive(controller->mutex);
}

void MasterSlavePIDSetOutput(MasterSlavePIDController *controller, int output)
{
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
		
	controller->enabledMasterPID = false;
	
	controller->manualMasterOutput = output;
	
	mutexGive(controller->mutex);
}

static void VivaLaRevolucion(MasterSlavePIDController *controller)
{
	// Judges, please talk to us about VIVA LA REVOLUCION!
	PIDController temp = controller->master;
	controller->master = controller->slave;
	controller->slave = temp;
}