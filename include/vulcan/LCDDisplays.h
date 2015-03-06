#ifndef LCD_DISPLAYS_H_
#define LCD_DISPLAYS_H_

#include "lcd/lcdmenu.h"

///@cond
char * getRobotState();
///@endcond

//constants
#define NUMTITLES	6
//external variable for the lcd menus
extern char *titles[NUMTITLES];
extern void (*exec[NUMTITLES])();
extern LCDMenu main_menu;

#endif