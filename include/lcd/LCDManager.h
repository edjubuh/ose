/**
* @file include/lcd/LCDManager.h
* @sa liblcd/LCDManager.c @link liblcd/LCDManager.c
*
* @htmlonly
* @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
* Portions of this file may contain elements from the PROS API. <br>
* See ReadMe.md (Main Page) for additional notice.
* @endhtmlonly
********************************************************************************/

#ifndef LCD_MANAGER_H_
#define LCD_MANAGER_H_

#include "lcd/LCDFunctions.h"

typedef struct
{
	char * (*GetText)(void);

	textJustifications justification;
} DisplayText;
///@cond
void initLCDManager();
void addCycleText(DisplayText, int);
void replaceCycleText(DisplayText, int, int);
///@endcond
#endif