/************************************************************************/
/* @file include/vulcan/lift.h											*/
/* @brief Header file for lift functions								*/
/* See vulcan/lift.c for details of all functions							*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef LIFT_H_
#define LIFT_H_

#include "sml/SmartMotorLibrary.h"

// ---------------- LEFT  SIDE ---------------- //
void LiftSetLeft(int, bool);

int LiftGetCalibratedPotentiometerLeft();

int LiftGetRawPotentiometerLeft();

int LiftGetEncoderLeft();

// ---------------- RIGHT SIDE ---------------- //
void LiftSetRight(int, bool);

int LiftGetCalibratedPotentiometerRight();

int LiftGetRawPotentiometerRight();

int LiftGetEncoderRight();

// ---------------- MASTER (ALL) ---------------- //
void LiftSet(int);

void LiftInitialize();

PIDController LiftPIDController_l, LiftPIDController_r;

#endif