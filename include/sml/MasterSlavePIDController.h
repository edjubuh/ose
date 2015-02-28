/**
 * @file include/sml/MasterSlavePIDController.h
 * @author Elliot Berman
 * @brief Header file for master/slave PID controller functions <br>
 * See libsml/MasterSlavePIDController.c for details of all functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef MASTER_SLAVE_PID_CONTROLLER_H_
#define MASTER_SLAVE_PID_CONTROLLER_H_

#include "main.h"
#include "sml/SmartMotorLibrary.h"

/**
 * @struct MasterSlavePIDController
 * Represents two related controllers that need to be synchronized for output.
 */
typedef struct
{
	/**
	 * @brief The PIDController of the master controller. The slave will attempt to synchronize with this controller
	 */
	PIDController master;
	/**
	 * @brief The PIDController struct of the slave controller. This controller will attempt to change its output to synchronize with the master controller
	 */
	PIDController slave;
	/**
	 * @brief The PIDController struct that controls the power of the synchronization of the slave controller.
	 */
	PIDController equalizer;
	/**
	 * @brief The maximum positive speed of the controller
	 */
	int maxSpeed;
	/**
	 * @brief The maximum negative speed of the controller
	 */
	int minSpeed;
	/**
	 * @brief A boolean representing whether or not the the master/slave PID control is enabled (synchronization will still apply).
	 */
	bool enabledPrimaryPID;
	/**
	 * @brief If enabledPrimaryPID = false, both controller outputs are set to this value (along with synchroniation).
	 */
	int manualPrimaryOutput;
} MasterSlavePIDController;
///@cond
MasterSlavePIDController CreateMasterSlavePIDController(PIDController, PIDController, PIDController, int, int, bool);
TaskHandle InitializeMasterSlaveController(MasterSlavePIDController*, int);
void MasterSlavePIDSetGoal(MasterSlavePIDController*, int);
void MasterSlavePIDSetOutput(MasterSlavePIDController*, int);
void MasterSlavePIDIncreaseGoal(MasterSlavePIDController*, int);
bool MasterSlavePIDOnTarget(MasterSlavePIDController*);
///@endcond
#endif