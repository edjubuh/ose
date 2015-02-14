/**
 * @file vulcan/auto.c		
 * @brief Source file for autonomous functions.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"

#include "vulcan/Chassis.h"
#include "vulcan/Lift.h"
#include "vulcan/ScoringMechanism.h"

/**
 * @brief Runs a sequence of commands during the competition "autonomous period." (15 seconds)
 */
void autonomous()
{
	/*
	//input before auton begins
	//left (-1) & right (1) is psuedo-code
	int side = 1;

	while (LiftSetHeight(125)) delay(35);
	ChassisSet(100, 100, false); //Go forward
	delay(750);
	ChassisSet(0, 0, false);
	delay(100);
	while (LiftSetHeight(0)) delay(35);
	delay(200);
	ChassisSet(side * 75, side * -75, false); //turn right/left?
	delay(250);
	ChassisSet(0, 0, false);
	while (LiftSetHeight(400)) delay(35); //height of the medium post???
	ChassisSet(50, 50, false);
	delay(100);
	ChassisSet(0, 0, false);
	ScoringMechSet(false); //deposit cube
	ChassisSet(-50, -50, false); //backup
	delay(200);
	ChassisSet(0, 0, false);
	while (LiftSetHeight(0)) delay(35);
	ChassisSet(side * 75, side * -75, false); //turn right/left?
	delay(250);
	ChassisSet(100, 100, false);
	delay(750);
	ChassisSet(0, 0, false); //return home!
	*/
	LiftSet(127, false);
	delay(2000);
	LiftSet(0, false);
	ChassisSet(-127, -127, false);
	delay(750);
	ChassisSet(0, 0, false);
	/*
	ChassisSet(127, 127, false);
	delay(500);
	ChassisSet(0, 0, false);
	delay(50);
	ChassisSet(-127, -127, false);
	delay(100);
	ChassisSet(0, 0, false);
	*/
}