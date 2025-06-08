/**
 TODO: cahnge view log into START BUTTON
 TODO: metti a posto l'output + bug nel GRAPH button
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <curses.h>     //graphic
#include <locale.h>     //setlocale()
#include <ctype.h>      //tolower()
#include <string.h>     //strlen()

#include "chronometer_engine.h"
#include "utils.h"      //util_save_activity()
#include "display.h"

/**Costant Variables**/
const int POWER_NAP = 9;

typedef enum {
    MENU_MODE,
    LABEL_MODE,
    GRAPH_MODE
} InputMode;

/**Main handle user inputs**/
int main(){
    ///Setting un curse.h
    setlocale(LC_ALL, "");          // UTF-8 and NOT-ASCII chars
    WINDOW* wmain = initscr();      // Start of ncurses app
    cbreak();                       // read char by char
    noecho();                       // No echo on stdout after getch() and derivate
    curs_set(FALSE);                // hide cursor
    timeout(0);                     // 0 means NON-BLOCKING READ
    
    ///Setting up User Interface
    if( display_setup(wmain) < 0 ) return -1;
    
    ///Useful variables for updating screen
    uint64_t chronoValue, chronoLastValue;
    
    ///Init the chronometer engine
    struct ChronoStatus cs;
    chrono_init(&cs);
    chrono_update(&cs);
    chronoLastValue = chrono_get_chrono_value(&cs);
    
    ///Setting un input mode
    InputMode inputMode = MENU_MODE;
    
    while(TRUE){
        ///Update chronometer and refreshing when needed
        chrono_update(&cs);
        chronoValue = chrono_get_chrono_value(&cs);
        if(chronoValue != chronoLastValue){
            chronoLastValue = chronoValue;
            display_clock_refresh(chronoValue);
        }
        
        ///Handling User Input
        switch(inputMode){
            ///Menu Mode: reading possibles menu command
            case MENU_MODE:
                {
                    int c = wgetch(stdscr);
                    if(c>=0) beep();
                    switch(tolower(c)){
                        case 'q':
                            if(chronoValue >= 3*60*10) ///BRO study for at least 3 min :)
                                util_save_activity(chronoValue, display_get_label() );
                            endwin();
                            return 0;
                            break;
                        case 'p':
                            chrono_toggle_pause(&cs);
                            break;
                        case 'e':
                            chrono_erase(&cs);
                            break;
                        case 'l':
                            ///Caso label, stampo la finestra, cambio la input mode e poi azzero il buffer
                            display_init_window_label();
                            inputMode = LABEL_MODE;
                            break;
                        case 'g':
                            display_init_window_graph();
                            inputMode = GRAPH_MODE;
                            break;
                    }
                }
                break;
            ///Label Mode:  getting the label string and update the chrono name
            case LABEL_MODE:
                if(display_handle_label_input()) inputMode = MENU_MODE;
                break;
            ///Graph Mode:
            case GRAPH_MODE:
                if(display_handle_graph_input()) inputMode = MENU_MODE;
                break;
            default:
                break;
        }
        napms(POWER_NAP); //Nap for 10 Millis! CPU will be gratefull :)
    }
    return 0;
}
