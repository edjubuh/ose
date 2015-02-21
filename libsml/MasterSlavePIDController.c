/**
 * @file libsml/MasterSlavePIDController.c
 * @author Elliot Berman
 * @brief Extension of SingleThreadPIDController to implement 
 *        a multitasked Master/Slave PID Controller.
 *        See addendum documentation for more details.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 **********************************************************************/

#include "main.h"
#include "sml/SingleThreadPIDController.h"

#include "sml/MasterSlavePIDController.h"
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

/**
 * @brief The task keeping the MasterSlavePIDController on target
 *
 * @param c
 *        A pointer to a MasterSlavePIDController
 */
static void MasterSlavePIDControllerTask(void *c)
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
		masterOutput -= PIDControllerCompute(equalizer);

		lcdprintf(Centered, 1, "eq:%d", PIDControllerCompute(equalizer));
		
		int max = 127;
		if(abs(masterOutput) > max)
			max = abs(masterOutput);
		if(abs(slaveOutput) > max)
			max = abs(slaveOutput);
		
		float scale = 127.0 / max;
		
		masterOutput = (int)(masterOutput * scale);
		slaveOutput  = (int)(slaveOutput * scale);
		
		master->Execute(masterOutput, false);
		slave->Execute(slaveOutput, false);
		
		mutexGive(controller->mutex);

		delay(50);
	}
}

/**
 * @brief Creates a MasterSlavePIDController struct based off of the parameters.
 *
 * @param master
 *			The controller for the master process. The call/execute methods should only affect the master. (i.e. the execute should not set the output for both slave and master)
 *
 * @param slave
 *			The controller for the slave process. The call/execute methods should only affect the slave system.
 *
 * @param equalizer
 *			The controller for the equalizer. The call/execute methods are not used. Only constants need to be tuned. <br>
 *			The equalizer will determine how much to change the slave's velocity in order to match the master.
 *
 * @param enabledPrimaryPID
 *			Enables the master/slave controllers. May be changed later by using MasterSlavePIDChangeGoal(), MasterSlavePIDIncreaseGoal(), or MasterSlavePIDSetOutput()
 *
 * @returns Returns a MasterSlavePIDController struct representing the arguments.
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
* @brief Initializes a Master/Slave PID Controller and creates the task for the controller
*
* @param controller
*        Point to a MasterSlavePIDController struct containing information for the controller
*
* @param primaryGoal
*        The primary PID controller goal height
*
* @return A TaskHandle of the created task (so the task may be stopped later)
*/
TaskHandle InitializeMasterSlaveController(MasterSlavePIDController *controller, int primaryGoal)
{
	controller->mutex = mutexCreate();
	mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT);
	controller->slave.Goal = primaryGoal;
	controller->master.Goal = primaryGoal;
	controller->manualPrimaryOutput = 0;
	mutexGive(controller->mutex);
	return taskCreate(MasterSlavePIDControllerTask, TASK_DEFAULT_STACK_SIZE, controller, TASK_PRIORITY_DEFAULT);
}

/**
 * @brief Changes the Primary PID Goal to the desired goal value.
 *
 * @param controller
 *        Point to a MasterSlavePIDController struct containing information for the controller
 *
 * @param primaryPIDGoal
 *        The new goal height
 */
void MasterSlavePIDSetGoal(MasterSlavePIDController *controller, int primaryPIDGoal)
{
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
	
	controller->enabledPrimaryPID = true;
	
	controller->master.Goal = primaryPIDGoal;
	controller->slave.Goal = primaryPIDGoal;
	
	mutexGive(controller->mutex);
}

/**
 * @brief Increments the MSPID primary controller by an integer value
 *        If the controller was previously in direct-to-output mode, the current height will be taken and then deltaGoal applied
 *
 * @param controller
 *        Point to a MasterSlavePIDController struct containing information for the controller
 *
 * @param deltaGoal
 *        The delta of the goal
 */
void MasterSlavePIDIncreaseGoal(MasterSlavePIDController *controller, int deltaGoal)
{
	//if we don't have enough resources at the current time, exit the function
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

/**
 * @brief Sets the direct-to-output value and begins direct-to-output mode, if not already in it
 *
 * @param controller
 *        Point to a MasterSlavePIDController struct containing information for the controller
 *
 * @param output
 *        The desired output value. [-127,127]
 */
void MasterSlavePIDSetOutput(MasterSlavePIDController *controller, int output)
{
	//if we don't have enough resources at the current time, exit the function
	if(!mutexTake(controller->mutex, MUTEX_TAKE_TIMEOUT))
		return;
		
	controller->enabledPrimaryPID = false;
	
	controller->manualPrimaryOutput = output;
	
	mutexGive(controller->mutex);
}

/**
 * @brief Judges, please talk to us about VIVA LA REVOLUCION!
 *
 * @param MasterSlavePIDController controller
 *			Revolutionizes the controller.
 */
static void VivaLaRevolucion(MasterSlavePIDController *controller)
{
	PIDController temp = controller->master;
	controller->master = controller->slave;
	controller->slave = temp;
}

