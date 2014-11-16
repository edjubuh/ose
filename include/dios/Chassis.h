/************************************************************************/
/* @file chassis.h		@brief Header file for chassis functions		*/
/* See src/chassis.c for details of all functions                       */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef CHASSIS_H_
#define CHASSIS_H_

void ChassisSet(int left, int right, bool immediate);

void ChassisInitialize();

#endif