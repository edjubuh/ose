#ifndef MECHOP_H
#define MECHOP_H
/**
 * @brief Constants
 */
#define STRAFE_CONST  254

/**
 * @brief Function declarations
 */
double toDegrees(double radians);
int toIntDegrees(double radians);
double getJoyTheta(unsigned int joystickNum);
int thetaSector( double theta );
int aJoy(int val1, int val2);
double cHypo(int x, int y);
int aHypo(double h1, double h2);
void JoystickControl();

#endif /*MECHOP_H*/