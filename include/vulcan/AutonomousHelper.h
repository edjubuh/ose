/**
* @file include/vulcan/AutonomousHelper.h
* @sa vulcan/auto.c @link vulcan/auto.c
* @sa vulcan/init.c @link vulcan/init.c
*
* @htmlonly
* @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
* Portions of this file may contain elements from the PROS API. <br>
* See ReadMe.md (Main Page) for additional notice.
* @endhtmlonly
********************************************************************************/

#ifndef AUTON_HELPER_H_
#define AUTON_HELPER_H_


#define RED_ALLIANCE		true
#define BLUE_ALLIANCE		false

#define SKYRISE_STARTING_TILE	true
#define POST_STARTING_TILE		false

extern int skyriseBuilt;


///@cond
void DeployScoringMech();
void BuildSkyrise();
void RunNoAutonomous();
void RunBlueSky();
void RunBlueCube();
void RunRedSky();
void RunRedCube();
void RunPSkills();
//@endcond
#endif