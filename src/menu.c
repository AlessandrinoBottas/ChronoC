#include <curses.h>
#include <string.h>
#include "menu.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

const int MENU_HEIGTH = 8;

char *cmd[] = {
    "[S]tart : Start the timer (DOES NOT SAVE)",
    "[L]abel : Set a label associated with the clock",
    "[P]ause : Pause/un-pause the clock",
    "[E]rase : Set the clock to 00:00:00:0 (DOES NOT SAVE)",
    "[G]raph : Show a graph based on saved data",
    "[Q]uit  : Exit the program and save",
    NULL    //sentinel
};

///Init the window of the menu
void menu_init(struct WindowMenu* wm){
    WINDOW* wmenu = wm->wmenu;
    
    box(wmenu,0,0);
    wattron(wmenu,A_BOLD);
    wattron(wmenu,A_BLINK);
    wattron(wmenu,A_REVERSE);
    mvwprintw(wmenu, 0, wm->xMid - strlen(" MENU ")/2, " MENU ");
    wattroff(wmenu,A_BLINK);
    wattroff(wmenu,A_REVERSE);
    wattroff(wmenu,A_BOLD);

    int cnt = 0;
    while (cmd[cnt] != NULL) cnt++;
    
    for(int i=0; cmd[i]!=NULL; i++){
        mvwprintw(wmenu, i+1, 1, "%c",cmd[i][0]);
        wattron(wmenu,A_BOLD);
        mvwprintw(wmenu, i+1, 2, "%c",cmd[i][1]);
        wattroff(wmenu,A_BOLD);
        mvwprintw(wmenu, i+1, 3, "%s",cmd[i]+2);
    }
}
