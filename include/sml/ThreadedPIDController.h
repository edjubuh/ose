/************************************************************************/
/* @file SingleThreadPIDController.h									*/
/* @brief Header file for single threaded PID controller functions		*/
/* See src/SingleThreadPIDController.c for details of all functions     */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef THREADED_PID_CONTROLLER_H_
#define THREADED_PID_CONTROLLER_H_

#include "sml/SmartMotorLibrary.h"

void ThreadedPIDControllerTask();

#endif