/**
 * @file include/vulcan/Lift.h		
 * @brief Header file for Lift functions <br>
 * See vulcan/Lift.c for details of all functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef LIFT_H_
#define LIFT_H_

#include "sml/SmartMotorLibrary.h"

// ---------------- LEFT  SIDE ---------------- //
void LiftSetLeft(int, bool);
int LiftGetCalibratedPotentiometerLeft();
int LiftGetRawPotentiometerLeft();

// ---------------- RIGHT SIDE ---------------- //
void LiftSetRight(int, bool);
int LiftGetCalibratedPotentiometerRight();
int LiftGetRawPotentiometerRight();

// ---------------- MASTER (ALL) ---------------- //
void LiftSet(int, bool);
void LiftSetHeight(int);
bool LiftContinuous();
int liftComputePotentiometerDifference();
void LiftInitialize();

PIDController LiftPIDController_l, LiftPIDController_r;

#endif