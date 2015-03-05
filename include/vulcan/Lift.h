/**
 * @file include/vulcan/Lift.h	
 * @brief Header file for Lift functions <br>
 * @sa vulcan/Lift.c @link vulcan/Lift.c
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#ifndef LIFT_H_
#define LIFT_H_

#include "sml/SmartMotorLibrary.h"
///@cond
// ---------------- LEFT  SIDE ---------------- //
void LiftSetLeft(int, bool);
int LiftGetCalibIMELeft();
int LiftGetRawIMELeft();
int LiftGetQuadEncLeft();
int LiftGetCalibPotLeft();
int LiftGetRawPotLeft();

// ---------------- RIGHT SIDE ---------------- //
void LiftSetRight(int, bool);
int LiftGetCalibIMERight();
int LiftGetRawIMERight();
int LiftGetQuadEncRight();
int LiftGetCalibPotRight();
int LiftGetRawPotRight();

// ---------------- MASTER (ALL) ---------------- //
void LiftSet(int, bool);
bool LiftSetHeight(int);
void LiftGoToHeightCompletion(int);
bool LiftGoToHeightContinuous(int);
void LiftInitialize();

PIDController LiftPIDController_l, LiftPIDController_r;
///@endcond
#endif