/**
* @file include/vulcan/AutonomousHelper.h
* @brief Header file containing miscelanous functions and variable to assist in execution and set up of autonomous
* @sa vulcan/auto.c @link vulcan/auto.c
* @sa vulcan/init.c @link vulan/init.c
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
#define BLUE_ALLIANCe		false

#define SKYRISE_STARTING_TILE	true
#define POST_STARTING_TILE		false

bool alliance;
bool startingTile;

int skyriseBuilt = 0;

///@cond
void DeployScoringMech();
void BuildSkyrise();
//@endcond
#endif