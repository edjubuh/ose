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
PIDController PIDControllerCreate(void(*func)(int, bool), int(*func)(void), double, double, double, int, int, int);

void PIDControllerReset(PIDController *);

int PIDControllerCompute(PIDController *);

int PIDControllerComputer(PIDController *, int);

bool PIDControllerExecuteContinuous(PIDController *);

void PIDControllerExecuteCompletion(PIDController *controller);

void PIDControllerSetGoal(PIDController *controller, int goal);

#endif