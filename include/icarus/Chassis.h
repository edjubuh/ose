/************************************************************************/
/* @file Chassis.h		@brief Header file for Chassis functions		*/
/* See src/Chassis.c for details of all functions                       */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef CHASSIS_H_
#define CHASSIS_H_

void ChassisSet(int left, int right, bool immediate);

void ChassisSetMecanum(double heading, int speed, int rotation, bool immediate);

void ChassisInitialize();

int ChassisGetLeftIME();

void ChassisInitialize();

#endif