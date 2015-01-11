/**
 * @file include/dios/chassis.h
 * @brief Header file for chassis functions <br>
 * See src/chassis.c for details of all functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef CHASSIS_H_
#define CHASSIS_H_

void ChassisSet(int, int, bool);

void ChassisSetMecanum(double, int, int, bool);

void ChassisInitialize();

#endif