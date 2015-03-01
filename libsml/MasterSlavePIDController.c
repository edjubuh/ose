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
		delay(15);

		masterOutput = controller->enabledPrimaryPID ? PIDControllerCompute(master) : controller->manualPrimaryOutput;
		slaveOutput = controller->enabledPrimaryPID ? PIDControllerCompute(slave) : controller->manualPrimaryOutput;
		
		slaveOutput += PIDControllerCompute(equalizer);
		masterOutput -= PIDControllerCompute(equalizer);
		
		if (masterOutput < controller->minSpeed || slaveOutput < controller->minSpeed)
		{
			if (masterOutput == slaveOutput)
			{
				masterOutput = controller->minSpeed;
				slaveOutput = controller->minSpeed;
			}
			else
			{
				double max = abs(controller->minSpeed);
				if (abs(masterOutput) > max)
					max = abs(masterOutput);
				if (abs(slaveOutput) > max)
					max = abs(slaveOutput);
				double scale = abs(controller->minSpeed) / max;

				masterOutput = (int)(masterOutput * scale);
				slaveOutput = (int)(slaveOutput * scale);
			}
		}
		else if (masterOutput > controller->maxSpeed || slaveOutput > controller->maxSpeed)
		{
			if (masterOutput == slaveOutput)
			{
				masterOutput = controller->maxSpeed;
				slaveOutput = controller->maxSpeed;
			}
			else
			{
				double max = abs(controller->maxSpeed);
				if (abs(masterOutput) > max)
					max = abs(masterOutput);
				if (abs(slaveOutput) > max)
					max = abs(slaveOutput);
				double scale = abs(controller->maxSpeed) / max;

				masterOutput = (int)(masterOutput * scale);
				slaveOutput = (int)(slaveOutput * scale);
			}
		}
		
		master->Execute(masterOutput, false);
		slave->Execute(slaveOutput, false);
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
 * @param max
 *			An artificial maximimum PWM value for both the master and slave. If the values are greater than value, master output and slave output will be scaled proportionally back
 *
 * @param min
 *			An artificial minimum PWM value for bothe the master and slave.
 *
 * @param enabledPrimaryPID
 *			Enables the master/slave controllers. May be changed later by using MasterSlavePIDChangeGoal(), MasterSlavePIDIncreaseGoal(), or MasterSlavePIDSetOutput()
 *
 * @returns Returns a MasterSlavePIDController struct representing the arguments.
 */
MasterSlavePIDController CreateMasterSlavePIDController(PIDController master, PIDController slave, PIDController equalizer, int max, int min, bool enabledPrimaryPID)
{
	MasterSlavePIDController controller;
	controller.master = master;
	controller.slave = slave;
	controller.equalizer = equalizer;
	controller.maxSpeed = max;
	controller.minSpeed = min;
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
	controller->slave.Goal = primaryGoal;
	controller->master.Goal = primaryGoal;
	controller->manualPrimaryOutput = 0;
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
	controller->enabledPrimaryPID = true;

	PIDControllerSetGoal(&(controller->master), primaryPIDGoal);
	PIDControllerSetGoal(&(controller->slave), primaryPIDGoal);
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
	controller->enabledPrimaryPID = false;
	controller->manualPrimaryOutput = output;
}

/**
 * @brief Returns true if the MasterSlavePIDController is on target
 */
bool MasterSlavePIDOnTarget(MasterSlavePIDController *controller)
{
	return
		((abs(controller->master.Goal - controller->master.Call()) < controller->master.AcceptableTolerance) &&
		(abs(controller->slave.Goal - controller->slave.Call()) < controller->slave.AcceptableTolerance));
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

