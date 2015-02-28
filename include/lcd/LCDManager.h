/**
* @file include/lcd/LCDManager.h
* @brief Header file for LCD Manager functions <br>
* See liblcd/LCDManager.c for details of all functions
*
* @copyright Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
* Portions of this file may contain elements from the PROS API. <br>
* See include/main.h for additional notice.
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