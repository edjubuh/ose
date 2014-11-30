/************************************************************************/
/* @file SingleThreadPIDController.c									*/
/* @brief Source file initialize functions.								*/
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "sml/SingleThreadPIDController.h"
#include <math.h>

/**
* Creates a PIDController struct based off of the parameters
* @param void(*Execute)(int,bool)
*		A function pointer to the output function. Takes a parameter int, the output, and a bool, the immediate value
*		Ex. void ChassisSet(int val, bool immediate); PIDControllerCreate(&ChassisSet, ...);
* @param int(*Call)(void)
*		A function pointer to the input function. Takes no parameters and returns the input
*		See ex. for void(*Execute)(int, bool)
* @param Kp
*		The proportional constant
* @param Ki
*		The integral constant
* @param Kd
*		The derivative constant
* @param MaxIntegral
*		Maximum value of the integral to prevent integral windup
* @param MinIntegral
*		Minimum value of the integral to prevent integral windup, usually the same magnitude as MinIntegral
* @param AcceptableTolerance
*		Tolerance of the controller
*/
PIDController PIDControllerCreate(void(*Execute)(int, bool), int(*Call)(void), double Kp, double Ki, double Kd, int MaxIntegral, int MinIntegral, int AcceptableTolerance)
{
	PIDController controller;
	controller.Execute = Execute;
	controller.Call = Call;
	controller.Kp = Kp;
	controller.Ki = Ki;
	controller.Kd = Kd;
	controller.MaxIntegral = MaxIntegral;
	controller.MinIntegral = MinIntegral;
	controller.AcceptableTolerance = AcceptableTolerance;
	return controller;
}

/**
* Resets the PIDController by setting the goal, integral, and prevError to 0
* @param controller
*		A pointer to the PIDController struct
*/
void PIDControllerReset(PIDController *controller)
{
	controller->Goal = 0;
	controller->integral = 0;
	controller->prevError = 0;
}

/**
* Computes and returns the output of one pass of the PID Controller
* @param controller
*		A pointer to the PIDController struct
*/
int PIDControllerCompute(PIDController *controller)
{
	int currError = controller->Goal - controller->Call();

	controller->integral += currError;
	if (controller->integral < controller->MinIntegral)
		controller->integral = controller->MinIntegral;
	else if (controller->integral > controller->MaxIntegral)
		controller->integral = controller->MaxIntegral;
	if (abs(currError) < abs(controller->AcceptableTolerance)) // 
		controller->integral = 0;


	long derivative = (currError - controller->prevError) /
		((micros() - controller->prevTime) * 1000000); // get true estimated instantaneous change in ticks/sec

	int out = (int)((controller->Kp * currError) + (controller->Ki * controller->integral) + (controller->Kd * derivative));


	if (abs(currError) < abs(controller->AcceptableTolerance))
		out = 0;
	
	controller->prevTime = micros();
	controller->prevError = currError;
	
	return out;
}

/**
* Computes and executes one pass of the PID Controller
* @param controller
*		A pointer to the PIDController struct
* @return True or false if the input parameter is within the acceptable tolerance of the goal
*/
bool PIDControllerExecuteContinuous(PIDController *controller)
{
	controller->Execute(PIDControllerCompute(controller), false);
	if (abs(controller->Goal - controller->Call()) < controller->AcceptableTolerance)
		return true;
	else return false;
}

/**
* Computes and executes the PIDController to completion
* @param controller
*		A pointer to the PIDController struct
*/
void PIDControllerExecuteCompletion(PIDController *controller)
{
	while (PIDControllerExecuteContinuous(controller))
		delay(DEFAULT_INTERVAL);
}

/**
* Resets the PIDController and sets the goal to parameter
* @param controller
*		A pointer to the PIDController struct
* @param goal
*		The goal value
*/
void PIDControllerSetGoal(PIDController *controller, int goal)
{
	PIDControllerReset(controller);
	controller->Goal = goal;
}