/************************************************************************/
/* @file SmartMotorLibrary.h		@brief Header file for SM functions	*/
/* See src/SmartMotorLibrary.c for details of all functions             */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef SMARTPIDLIBRARY_H_
#define SMARTPIDLIBRARY_H_

#define DEFAULT_SKEW 0.5 // default SkewPerMsec
#define MUTEX_TAKE_TIMEOUT	2000 // default timeout when attempting to take a mutex

typedef struct
{
	void(*Execute)(int, bool);
	int(*Call)(void);

	int Goal;

	double	Kp,
		Ki,
		Kd;

	int MaxIntegral,
		MinIntegral;

	unsigned int AcceptableTolerance;

	// For internal use only
	int integral,
		// For internal use only
		prevError;
	// For internal use only
	unsigned long prevTime;
} PIDController;

typedef struct
{
	// This pointer (usually NULL) will recalculate the commanded speed
	int(*RecalculateCommanded)(int);
	unsigned char channel;

	bool inverted;

	int commanded;

	double skewPerMsec;
	long lastUpdate;
} Motor;

void InitializeMotorManager();

void StopMotorManager();

void MotorManagerTask(void *);

void MotorConfigure(int, bool, double);

void MotorChangeRecalculateCommanded(int, int(*foo)(int));

bool MotorSet(int, int, bool);

int MotorGet(int);

#endif