/**
 * @file vulcan/Chassis.h
 * @author Elliot Berman and Robert Shrote
 * @brief Header file for Chassis functions <br>
 * See vulcan/Chassis.c for details of all functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef CHASSIS_H_
#define CHASSIS_H_
///@cond
// ---------------- LEFT  SIDE ---------------- //
void ChassisSetLeft(int, bool);
int ChassisGetIMELeft();
int ChassisGetIRLeft();

// ---------------- RIGHT SIDE ---------------- //
void ChassisSetRight(int, bool);
int ChassisGetIMERight();
int ChassisGetIRRight();

// ---------------- MASTER (ALL) ---------------- //
void ChassisSet(int, int, bool);
void ChassisSetMecanum(double, int, int, bool);
bool ChassisGotToGoalContinuous(int, int);
void ChassisGoToGoalCompletion(int, int);
void ChassisInitialize();
///@endcond
#endif