#include "lcd/lcdmenu.h"

/**
 * @brief Creates an LCDMenu struct for use in rotating through menus
 * 
 * @param numTitles
 *        The number of titles. The number of titles should be the same as
 *        the number of function pointers. This variable is used to constrain
 *        arrays.
 * 
 * @param *titles[]
 *        Pointer to an array of strings for the titles. Array should be of
 *        length numTitles.
 * 
 * @param (**function)()
 *        Pointer to an array of function pointers. Function pointers should
 *        directly correspond with its respective titles. Array is of length
 *        numTitles.
 * 
 * @return An initialized LCDMenu struct.
 */
LCDMenu lcdmenuCreate(int numTitles, char *titles[] , void (**function)())
{
    //create menu struct
    LCDMenu menu;
    
    menu.titles = titles;       //set pointer (*titles[]) to **titles in struct
    menu.foo = function;        //set pointer (**function)() to (**foo)() in struct
    menu.numTitles = numTitles; //set constraints for arrays above
    menu.display = 0;           //initialize diplay...
    menu.execute = 0;           //...and execute to 0, the default
    
    return menu;
}

/**
 * @brief Updates the LCD screen to display the current title.
 * 
 * @param *menu
 *        A pointer to the LCDMenu struct
 */
void lcdmenuDisplay(LCDMenu *menu)
{
    lcdprintf(Left, 1, "%s", menu->titles[display]);
    return;
}


/**
 * @brief Shifts the position of the LCDMenu and updates the display
 * 
 * @param *menu
 *        A pointer to the LCDMenu struct
 * @param shift
 *        The number of shifts to make.
 *        @note shift range is -numTitles exclusive to 127 (maximum unsigned
 *              char value) inclusive.
 *        @note For safety use the #defined values of LCD_LEFT & LCD_RIGHT
 */
#define LCD_LEFT -1
#define LCD_RIGHT 1
//only use defined LCD_LEFT and LCD_RIGHT
void lcdmenuShift(LCDMenu *menu, signed char shift)
{
    //below line will not work if shift is less than negative menu->numTitles
    menu->display = (unsigned char)( (menu->display + (menu->numTitles + shift) ) % menu->numTitles);
    lcdmenuDisplay(menu);
    return;
}

/**
 * @brief Sets the execute index to the current display title index.
 * 
 * @param *menu
 *        A pointer to the LCDMenu struct.
 */
void lcdmenuDecide(LCDMenu *menu)
{
    menu->execute = menu->display;
    return;
}

/**
 * @brief Executes the function for the LCD option decided by the user.
 * 
 * @param *menu
 *        A pointer to the LCDMenu struct
 */
void lcdmenuExecute(LCDMenu *menu)
{
    menu->foo[menu->execute]();
    return;
}
