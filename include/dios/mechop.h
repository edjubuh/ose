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