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
 * Function declarations
 */
LCDMenu lcdmenuCreate(int numTitles, char *titles[] , void (**function)());
void lcdmenuDisplay(LCDMenu *menu);
void lcdmenuShift(LCDMenu *menu, signed char shift);
void lcdmenuDecide(LCDMenu *menu);
void lcdmenuExecute(LCDMenu *menu);