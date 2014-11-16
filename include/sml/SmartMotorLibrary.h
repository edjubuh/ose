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

typedef double(*SkewFunc)();
typedef struct
{
	unsigned char channel;

	bool inverted;

	int commanded;

	double skewPerMsec;
	long lastUpdate;
} Motor;

void InitializeMotorManager();

void MotorManagerTask(void *none);

void MotorConfigure(int channel, bool inverted, double skewPerMsec);

bool MotorSet(int channel, int set, bool immediate);


#endif