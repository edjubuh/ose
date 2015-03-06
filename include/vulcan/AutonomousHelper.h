/**
* @file include/vulcan/AutonomousHelper.h
* @brief Header file containing miscelanous functions and variable to assist in execution and set up of autonomous
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

#include "lcd/lcdmenu.h"

#define RED_ALLIANCE		true
#define BLUE_ALLIANCE		false

#define SKYRISE_STARTING_TILE	true
#define POST_STARTING_TILE		false

extern bool runAutonomous;
extern bool alliance;
extern bool startingTile;
extern bool runPSkills;

extern int skyriseBuilt;

extern LCDMenu main_menu;

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