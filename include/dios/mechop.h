/**
 * @file include/dios/mechop.h
 * @brief Header file for advanced mecanum control <br>
 * See dios/mechop.c for details of all functions
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef MECHOP_H
#define MECHOP_H
/**
 * @brief Constants
 */
#define STRAFE_CONST  254

/**
 * @brief Function declarations
 */
double toDegrees(double);

int toIntDegrees(double);

double getJoyTheta(int, int);

int thetaSector(double);

int aJoy(int, int);

double cHypo(int, int);

int aHypo(double, double);

void JoystickControl();

#endif /*MECHOP_H*/