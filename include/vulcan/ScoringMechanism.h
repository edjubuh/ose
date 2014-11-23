/************************************************************************/
/* @file ScoringMechanism.h	@brief Header file for ScoringMech functions*/
/* See src/ScoringMechanism.c for details of all functions              */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#ifndef SCORINGMECHANISM_H_
#define SCORINGMECHANISM_H_

/// <summary>
/// Sets the scoring mechanism PWM output to the value
/// </summary>
/// <param name='value'>The PWM value. Will be checked for bounds</param>
/// <param name='immediate'>Indicates if PWM change needs to happen immediately. Usually false, will ramp speeds</param>
void ScoringMechSet(bool value);

void ScoringMechSwitch();

#endif