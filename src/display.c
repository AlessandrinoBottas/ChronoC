#include "display.h"
#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>      //tolower(), isprint()
#include "menu.h"
#include "clock.h"
#include "utils.h"

/**Global vars**/
WINDOW* wmain;

int xMax, yMax;         //Max window size
int xIsOdd,yIsOdd;      //is xMax or yMax is odd number (needed for centering)

struct WindowClock wc;
struct WindowMenu  wm;
struct WindowLabel wl;
struct WindowGraph wg;

char buff[LABEL_LEN];
int  buffIndex = 0;

/**Functions Firms**/
int   display_setup(WINDOW* wmain);                     ///Setup all the user interface

///Menu
int   display_init_window_menu();                       ///Called by display_setup( ), setup the menu window
///Menu - Graph
int   display_init_window_graph();                      ///Setup for graph window
int   display_handle_graph_input();                     ///Handle user input while in GRAPH_MODE
void  display_delete_window_graph();                    ///Delete the graph window
///Menu - Label
int   display_init_window_label();
int   display_handle_label_input();
void  display_delete_window_label();
void  display_update_label_input_text();

///Clock
int   display_init_window_clock();                      ///Called by display_setup( ), setup the clock window
void  display_set_window_clock_label();
void  display_clock_refresh(uint64_t chronoValue);      ///Refresh the clock numbers

///Other
char* display_get_label();

                           
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
    
    menu_init(&wm);
    
    return 0;
}

int display_init_window_graph() {
    char title[] = "GRAPH";
    wg.y = 0;
    wg.x = 0;
    wg.nlines = wm.nlines;
    wg.ncols = wm.ncols;
    wg.xMid = wm.xMid;
    
    /// Create derived window inside the menu
    wg.wgraph = derwin(wm.wmenu, wg.nlines, wg.ncols, wg.y, wg.x);  if (!wg.wgraph) return -1;
    
    wtimeout(wg.wgraph,0);  //ofc non blocking since we want to update the clock
    
    ///Erase for delete garbage of previous pages, write the box
    werase(wg.wgraph);
    box(wg.wgraph, 0, 0);
    
    ///Write Title
    wattron(wg.wgraph,A_BOLD);
    wattron(wg.wgraph,A_BLINK);
    wattron(wg.wgraph,A_REVERSE);
    mvwprintw(wg.wgraph, 0, wg.xMid - strlen(title)/2 - strlen(title)%2, " %s ",title);       //scrivo il titolo
    wattroff(wg.wgraph,A_BLINK);
    wattroff(wg.wgraph,A_REVERSE);
    wattroff(wg.wgraph,A_BOLD);
    
    char** datas = util_retrieve_data_for_graph();
    
    if(datas != NULL)
        for(int i=0; datas[i]!=NULL; i++)
            mvwprintw(wg.wgraph, i+1, 1, "%s",datas[i]);
    else mvwprintw(wg.wgraph, 1, 1, "datas retrieved from file = (null)");
    
    
    free(datas);
    wrefresh(wg.wgraph);
    touchwin(wm.wmenu);
    wrefresh(wm.wmenu);
    return 0;
}

void display_delete_window_graph() {
    werase(wm.wmenu);
    box(wm.wmenu, 0, 0);
    menu_init(&wm);
    wrefresh(wm.wmenu);
    wg.wgraph = NULL;       //delwin cause segmentation fault!!! -> maybe for touchiwn?
}

/**---------------------------------------Label Sub-Window-----------------------------------------------**/
int display_init_window_label(){
    char title[] = "Insert label name";
    ///Init buff for label input
    for(int i=0; i<LABEL_LEN; i++) buff[i] = '_';
    buff[LABEL_LEN - 1] = '\0';
    buffIndex = 0;
    
    ///Set-Up for Label Sub-Window
    wl.y = 1;                           //y position
    wl.x = 2;                           //x position
    wl.nlines = 3;                      //heigth
    wl.ncols = xMax - 4;                //width
    wl.xMid = (wl.ncols - xIsOdd)/2;    //middle x

    /// Create derived window inside the menu
    wl.wlabel = derwin(wm.wmenu, wl.nlines, wl.ncols, wl.y, wl.x);  if (!wl.wlabel) return -1;

    wtimeout(wl.wlabel,0);  //ofc non blocking since we want to update the clock
    
    ///Erase for delete garbage of previous pages and write the box
    werase(wl.wlabel);
    box(wl.wlabel, 0, 0);
    
    ///Write Title
    wattron(wl.wlabel,A_BOLD);
    wattron(wl.wlabel,A_BLINK);
    wattron(wl.wlabel,A_REVERSE);
    mvwprintw(wl.wlabel, 0, wl.xMid - strlen(title)/2 - strlen(title)%2, " %s ",title);
    wattroff(wl.wlabel,A_BLINK);
    wattroff(wl.wlabel,A_REVERSE);
    wattroff(wl.wlabel,A_BOLD);
    
    ///Write Placeholder
    mvwprintw(wl.wlabel, 1, wl.xMid - strlen(buff)/2, "%s", buff);
    
    /// Update the changes
    wrefresh(wl.wlabel);
    touchwin(wm.wmenu);
    wrefresh(wm.wmenu);

    return 0;
}

void display_delete_window_label() {
    werase(wm.wmenu);
    box(wm.wmenu, 0, 0);
    menu_init(&wm);
    wrefresh(wm.wmenu);
    untouchwin(wl.wlabel);  //cause we touched ;)
    wl.wlabel = NULL;       //delwin cause segmentation fault!!!
}

void display_set_window_clock_label(){
    ///Writing label
    box(wc.wclock,0,0);         //this erase the previous name
    
    wattron(wc.wclock,A_BOLD);
    wattron(wc.wclock,A_BLINK);
    wattron(wc.wclock,A_REVERSE);
    mvwprintw(wc.wclock, 0, wc.xMid - strlen(buff)/2, " %s ",buff);
    wattroff(wc.wclock,A_BLINK);
    wattroff(wc.wclock,A_REVERSE);
    wattroff(wc.wclock,A_BOLD);
}

int display_handle_label_input(){
    ///Read the char and handle the input
    char ch = wgetch(wl.wlabel);
    if(ch == ERR) return 0;
    
    if(ch == '\n' || buffIndex > (LABEL_LEN-2) ){
        buff[buffIndex] = '\0';
        display_set_window_clock_label();
        display_delete_window_label();
        return 1;
    }else if(ch == 127 && buffIndex > 0){           //127 = backspace
        buff[--buffIndex] = '_';
        display_update_label_input_text();
    }else if(isprint(ch)){
        buff[buffIndex++] = ch;
        display_update_label_input_text();
    }
    return 0;
}

char* display_get_label(){ return buff; }

int display_handle_graph_input(){
    char ch = wgetch(wg.wgraph);                            //prendo il carattere in input
    if(ch == ERR) return 0;
    if(ch){
        display_delete_window_graph();
        return 1;
    }
    return 1;
}

void  display_update_label_input_text(){
    mvwprintw(wl.wlabel, 1, wl.xMid - strlen(buff)/2, "%s", buff);
    
    wrefresh(wl.wlabel);
    touchwin(wm.wmenu);
    wrefresh(wm.wmenu);
}
