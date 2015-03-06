/**
 * @file include/sml/SingleThreadPIDController.h
 * @author Elliot Berman
 * @sa libsml/SingleHreadPIDController.c @link libsml/SingleThreadPIDController.c
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#ifndef SINGLETHREADPID_H_
#define SINGLETHREADPID_H_

#include "sml/SmartMotorLibrary.h"

#define DEFAULT_INTERVAL 20
///@cond
PIDController PIDControllerCreate(void(*e)(int, bool), int(*c)(void), double, double, double, int, int, int);
void PIDControllerReset(PIDController *);
int PIDControllerCompute(PIDController *);
int PIDControllerComputer(PIDController *, int);
bool PIDControllerExecuteContinuous(PIDController *);
void PIDControllerExecuteCompletion(PIDController *controller);
void PIDControllerSetGoal(PIDController *controller, int goal);
///@endcond
#endif