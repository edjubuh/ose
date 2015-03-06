/**
 * @file include/vulcan/ScoringMechanism.h
 * @brief Header file for ScoringMech functions <br>
 * @sa vulcan/ScoringMechanism.c @link vulcan/ScoringMechanism.c
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#ifndef SCORINGMECHANISM_H_
#define SCORINGMECHANISM_H_
///@cond
//------- SCORING NEEDLE ------ //
void ScoringMechNeedleSet(bool);
void ScoringMechNeedleSwitch();
bool ScoringMechNeedleGet();

//-------- SCORING CLAW ------- //
void ScoringMechClawSet(bool);
void ScoringMechClawSwitch();
bool ScoringMechClawGet();
///@endcond
#endif