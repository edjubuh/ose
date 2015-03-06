/**
* @file include/lcd/lcdmenu.h
* @brief Header fiel for the LCD menu functionality of liblcd
* @sa liblcd/lcdmenu.c @link liblcd/lcdmenu.c
*
* @htmlonly
* @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
* Portions of this file may contain elements from the PROS API. <br>
* See ReadMe.md (Main Page) for additional notice.
* @endhtmlonly
********************************************************************************/

#ifndef LCDMENU_H_
#define LCDMENU_H_

/**
 * @struct lcd_menu
 *         A struct representing a menu. Contains pointers to titles
 *         and corresponding functions. Contains constants and other
 *         variables needed to service the lcdmenu...(...) functions.
 */
struct lcd_menu {
    char **titles;              //array of titles to display
    void (**foo)();             //pointer to array of function pointers (same number as titles)
    unsigned char numTitles;    //number of titles; used to constrain array indexes.
    unsigned char display;      //what title to display
    unsigned char execute;      //what title/function to execute
};

typedef struct lcd_menu LCDMenu;

/**
 * Constants
 */
#define LCD_SHIFT_LEFT -1
#define LCD_SHIFT_RIGHT 1

///@cond
/**
 * Function declarations
 */
LCDMenu lcdmenuCreate(int numTitles, char *titles[] , void (**function)());
void lcdmenuDisplay(LCDMenu *menu);
void lcdmenuShift(LCDMenu *menu, signed char shift);
void lcdmenuDecide(LCDMenu *menu);
void lcdmenuExecute(LCDMenu *menu);
///@endcond
#endif /*LCDMENU_H_*/