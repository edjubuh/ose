/************************************************************************/
/* @file SingleThreadPIDController.h									*/
/* @brief Header file for single threaded PID controller functions		*/
/* See src/SingleThreadPIDController.c for details of all functions     */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef SINGLETHREADPID_H_
#define SINGLETHREADPID_H_

#include "sml/SmartMotorLibrary.h"

#define DEFAULT_INTERVAL 20

// Creates a PID Controller struct
PIDController PIDControllerCreate(void(*Execute)(int, bool), int(*Call)(void), double Kp, double Ki, double Kd, int MaxIntegral, int MinIntegral, int AcceptableTolerance);

// Cleanses internal variables in a PID Controller 
void PIDControllerReset(PIDController *controller);

// Computes and returns a PID Controller
int PIDControllerCompute(PIDController *controller);

int PIDControllerComputer(PIDController *controller, int error);

// Executes a PID Controller once, for use when running multiple PID Controllers at once on the same level
bool PIDControllerExecuteContinuous(PIDController *controller);

// Executes a PID Controller until it has reached target
void PIDControllerExecuteCompletion(PIDController *controller);

// Sets the goal of a PID Controller
void PIDControllerSetGoal(PIDController *controller, int goal);

#endif