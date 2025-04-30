#include "display.h"
#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "clock.h"

/**Global vars**/
WINDOW* wmain;

int xMax, yMax;
int xIsOdd,yIsOdd;

struct WindowClock wc;
struct WindowMenu  wm;
struct WindowLabel wl;

char s[] = "Insert label name: ";

/**Functions**/
int display_init_window_clock();
int display_init_window_menu();


/**Function Implementations**/
int display_setup(WINDOW* wmain){
    ///Getting params
    xMax = getmaxx(wmain);
    yMax = getmaxy(wmain);
    xIsOdd = xMax%2;
    yIsOdd = yMax%2;
    
    ///Min sizes requirements
    if(yMax < CLOCK_HEIGTH + MENU_HEIGTH + 2 || xMax < CLOCK_WIDTH){
        fprintf(stderr, "Window size at least: %d,%d (CURRENT %d,%d)\n", CLOCK_HEIGTH + MENU_HEIGTH + 2, CLOCK_WIDTH, xMax, yMax);
        return -1;
    }
    
    ///Init windows
    if( display_init_window_clock()<0 || display_init_window_menu()<0){
        fprintf(stderr, "Error while creating CLOCK and MENU windows");
        return -1;
    }
    
    ///Commit changes
    wrefresh(wmain);
    touchwin(wc.wclock);    //because digits are subwindows of clock window
    wrefresh(wc.wclock);
    wrefresh(wm.wmenu);
    
    return 0;
}


///Refresh the clock window by updating the digits and printing them
void display_clock_refresh(uint64_t chronoValue){
    clock_set_time(chronoValue);    //set new digits
    clock_update();                 //write new digits
    touchwin(wc.wclock);            //commit changes
    wrefresh(wc.wclock);            //commit changes
}

int display_init_window_clock(){
    wc.x=0;
    wc.y=0;
    wc.nlines = yMax - MENU_HEIGTH;
    wc.ncols = xMax;
    wc.xMid = (xMax - xIsOdd)/2;
    wc.yMid = (wc.nlines - yIsOdd)/2;
    wc.wclock = newwin(wc.nlines, wc.ncols, wc.y, wc.x);    if(wc.wclock<0) return -1;
    
    clock_windows_init(&wc);
    clock_set_time(0);
    clock_update();
    
    return 0;
}

int display_init_window_menu(){
    wm.x = 0;
    wm.y = wc.nlines;
    wm.nlines = MENU_HEIGTH;
    wm.ncols = xMax;
    wm.xMid = (xMax - xIsOdd)/2;
    wm.yMid = (MENU_HEIGTH)/2;
    wm.wmenu = newwin(wm.nlines, wm.ncols, wm.y, wm.x); if(wm.wmenu<0) return -1;
    
    wtimeout(wl.wlabel, 0);
    curs_set(TRUE);
    keypad(wl.wlabel, TRUE);
    
    menu_init(&wm);
    
    return 0;
}

int display_init_window_label() {
    wl.y = 1;
    wl.x = 2;
    wl.nlines = 3;
    wl.ncols = xMax - 4;

    /// Create derived window inside the menu
    wl.wlabel = derwin(wm.wmenu, wl.nlines, wl.ncols, wl.y, wl.x);  if (!wl.wlabel) return -1;

    wtimeout(wl.wlabel,0);  //ofc non blocking since we want to update the clock
    
    ///Make space, draw border and prompt
    werase(wl.wlabel);
    box(wl.wlabel, 0, 0);
    keypad(wl.wlabel, TRUE);
    
    mvwprintw(wl.wlabel, 1, 1, "%s",s);
    wmove(wl.wlabel, 1, strlen(s) + 1);
    
    /// Configure input settings
    wrefresh(wl.wlabel);
    touchwin(wm.wmenu);
    wrefresh(wm.wmenu);

    return 0;
}

void display_delete_window_label(void) {
    werase(wm.wmenu);
    box(wm.wmenu, 0, 0);
    menu_init(&wm);
    wrefresh(wm.wmenu);
    wl.wlabel = NULL; //delwin cause segmentation fault!!!
}

WINDOW* display_get_window_label(void) { return wl.wlabel;}

void display_set_window_clock_name(char* s){
    int len = strlen(s);
    ///Writing label
    wattron(wc.wclock,A_BOLD);
    wattron(wc.wclock,A_BLINK);
    wattron(wc.wclock,A_REVERSE);
    mvwprintw(wc.wclock, 0, wc.xMid - len/2, " %s ",s);
    wattroff(wc.wclock,A_BLINK);
    wattroff(wc.wclock,A_REVERSE);
    wattroff(wc.wclock,A_BOLD);
}

void display_update_label_input_text(char* label){
    //dunno how to do it so instead of deleting I first print blank chars
    //and then i print the actual label
    //P.S. i could rewrite all
    char* blank = calloc(xMax-4 - strlen(s), sizeof(char));
    for(int i=0; i<xMax-4 - strlen(s)-2; i++) blank[i] = ' ';
    blank[xMax-4 - strlen(s)-1] = '\0';
    mvwprintw(wl.wlabel, 1, strlen(s)+1, "%s", blank);
    mvwprintw(wl.wlabel, 1, strlen(s)+1, "%s", label);
    wrefresh(wl.wlabel);
}
