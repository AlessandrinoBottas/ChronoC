#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <curses.h>     //graphic
#include <locale.h>     //setlocale()
#include <sys/time.h>   //get_time() ->DELETE???!?!?!?
#include <ctype.h>      //tolower()
#include <string.h>     //strlen() ->DELETE??!?!?!

#include "chronometer_engine.h"
#include "utils.h"
#include "display.h"
#include "clock.h"

/**Costant Variables**/
const int DELAY = 9;
const int LABEL_LEN = 50;

/**Global Variables**/
char labelBuffer[LABEL_LEN] = {'\0'};
int  labelIndex = 0;

typedef enum {
    MENU_MODE,
    LABEL_MODE
} InputMode;

/**Main handle user inputs**/
int main(){
    setlocale(LC_ALL, "");          // UTF-8 and NOT-ASCII chars
    WINDOW* wmain = initscr();      // Start of ncurses app
    cbreak();                       // lettura carattere per carattere
    noecho();                       // No echo on stdout after getch() and derivate
    curs_set(FALSE);                // hide cursor
    timeout(0);
    
    ///Setting up Uuser Interface
    if( display_setup(wmain) < 0 ) return -1;
    
    ///Init the chronometer engine
    struct ChronoStatus cs;
    chrono_init(&cs);
    chrono_update(&cs);
    uint64_t chronoLastValue = chrono_get_chrono_value(&cs);
    
    ///Setting un input mode
    InputMode inputMode = MENU_MODE;
    
    while(TRUE){
        ///Synchronize chronometer and refreshing
        chrono_update(&cs);
        uint64_t chronoValue = chrono_get_chrono_value(&cs);
        if(chronoValue != chronoLastValue){
            chronoLastValue = chronoValue;
            display_clock_refresh(chronoValue);
        }
        
        switch(inputMode){
            case MENU_MODE:
                {
                    int c = wgetch(stdscr);
                    if(c>=0) beep();
                    switch(tolower(c)){
                        case 'q':
                            if(clock_get_time() > 5*60*10)//at least 5 min :)
                                util_save_activity(clock_get_time(), labelBuffer);
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
                            display_init_window_label();
                            inputMode = LABEL_MODE;
                            for(int i=0; i<LABEL_LEN; i++) labelBuffer[i] = '\0';
                            labelIndex = 0;
                            break;
                    }
                }
                break;
            case LABEL_MODE:
                {
                    WINDOW* w = display_get_window_label();
                    char ch = wgetch(w);
                    if(ch == ERR) break;
                    else if(ch == 127){
                        --labelIndex;
                        labelBuffer[labelIndex] = '\0';
                        display_update_label_input_text(labelBuffer);
                    }else if(ch == '\n'){   //127 = backspace
                        inputMode = MENU_MODE;
                        display_set_window_clock_name(labelBuffer);
                        display_delete_window_label();
                    }else if(isprint(ch)){
                        labelBuffer[labelIndex++] = ch;
                        display_update_label_input_text(labelBuffer);
                    }
                }
                break;
            default:
                break;
        }
        napms(DELAY); //Nap for 10 Millis! CPU will be gratefull :)
    }

    return 0;
}

