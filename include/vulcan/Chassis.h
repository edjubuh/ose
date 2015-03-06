/**
 * @file vulcan/Chassis.h
 * @author Elliot Berman and Robert Shrote
 * @brief Header file for Chassis functions <br>
 * @sa vulcan/Chassis.c @link vulcan/Chassis.c
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#ifndef CHASSIS_H_
#define CHASSIS_H_

#define CHASSIS_IR_RIGHT_RED_THRESH		200
#define CHASSIS_IR_LEFT_RED_THRESH		200
#define CHASSIS_IR_RIGHT_GREY_THRESH	700
#define CHASSIS_IR_LEFT_GREY_THRESH		700
#define CHASSIS_IR_RIGHT_BLUE_THRESH	200 //!@todo: Find this value
#define CHASSIS_IR_LEFT_BLUE_THRESH		200 //!@todo: Find this value

typedef enum
{
	Red,
	Blue,
	Grey
} kTiles;

///@cond
// ---------------- LEFT  SIDE ---------------- //
void ChassisSetLeft(int, bool);
int ChassisGetIMELeft();
int ChassisGetIRLeft();
bool ChassisHasIRLineLeft(kTiles);

// ---------------- RIGHT SIDE ---------------- //
void ChassisSetRight(int, bool);
int ChassisGetIMERight();
int ChassisGetIRRight();
bool ChassisHasIRLineRight(kTiles);

// ---------------- MASTER (ALL) ---------------- //
void ChassisSet(int, int, bool);
void ChassisSetMecanum(double, int, int, bool);
void ChassisResetIMEs();
bool ChassisGoToGoalContinuous(int, int);
void ChassisGoToGoalCompletion(int, int);
void ChassisAlignToLine(int, int, kTiles);
void ChassisInitialize();
///@endcond
#endif