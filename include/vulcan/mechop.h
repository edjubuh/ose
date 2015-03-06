/**
 * @file include/vulcan/mechop.h
 * @sa vulcan/mechop.c @link vulcan/mechop.c
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#ifndef MECHOP_H
#define MECHOP_H
#define STRAFE_CONST	254
#define THRESHOLD		25
///@cond
double getJoyTheta(int, int);
int thetaSector(double);
int aJoy(int, int);
double cHypo(int, int);
int aHypo(double, double);
void JoystickControl(int, int, int, int);
///@endcond
#endif /*MECHOP_H*/