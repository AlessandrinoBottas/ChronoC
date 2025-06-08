#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"

void print_digit(WINDOW* w, int n);

/**Costant Variables**/
const int COLON_HEIGTH = 8;
const int DIGIT_HEIGTH = 8;
const int COLON_WIDTH = 5;
const int DIGIT_WIDTH = 11;
const int NUM_DGT = 7;          // number of digits in the clock
const int NUM_SEP = 3;          // number of separator (colons) in the clock
const int CLOCK_WIDTH = NUM_DGT * DIGIT_WIDTH + NUM_SEP * COLON_WIDTH;
const int CLOCK_HEIGTH = DIGIT_HEIGTH;


/**Struct definition**/
struct Digit     { WINDOW* w; };
struct Separator { WINDOW* w; };


/**Global Vars**/
static struct Digit     d[NUM_DGT];
static struct Separator s[NUM_SEP];
static int v[NUM_DGT];
static int lastVal[NUM_DGT];


/**Functions implementations**/

///Create the windows for digits and separators from a starting position
int clock_windows_init(struct WindowClock* wc) {
    char ss[] = " GIMME A LABEL ";
    
    ///Draw border
    box(wc->wclock, 0, 0);
    
    ///Setting starting points
    int xClock = wc->xMid - CLOCK_WIDTH/2;
    int yClock = wc->yMid - CLOCK_HEIGTH/2;

    ///Writing label of the clock
    wattron(wc->wclock,A_BOLD);
    wattron(wc->wclock,A_BLINK);
    wattron(wc->wclock,A_REVERSE);
    mvwprintw(wc->wclock, 0, wc->xMid - strlen(ss)/2, "%s", ss);
    wattroff(wc->wclock,A_BLINK);
    wattroff(wc->wclock,A_REVERSE);
    wattroff(wc->wclock,A_BOLD);
    
    ///Drawing subwindows for digits and colons
    int cnt = 0;
    for (int i = 0; i < NUM_DGT + NUM_SEP; i++) {
        if (i % 3 == 2) {
            s[cnt].w = derwin(wc->wclock, COLON_HEIGTH, COLON_WIDTH, yClock, xClock);
            if (!s[cnt].w) return -1;
            xClock += COLON_WIDTH;
            cnt++;
        } else {
            int k = i - cnt;
            d[k].w = derwin(wc->wclock, DIGIT_HEIGTH, DIGIT_WIDTH, yClock, xClock);
            if (!d[k].w) return -1;
            xClock += DIGIT_WIDTH;
        }
    }
    
    ///Init vars (doens't work without it)
    for (int i = 0; i < NUM_DGT; i++) {
        v[i] = 0;
        lastVal[i] = -1;
    }
    
    return 0;
}

///Refresh digits
void clock_update(void) {
    static int flag = 1;

    for (int i = 0; i < NUM_DGT; i++) {
        if (lastVal[i] == v[i]) continue;

        print_digit(d[i].w, v[i]);
        lastVal[i] = v[i];
    }

    if (flag) {
        for (int i = 0; i < NUM_SEP; i++) print_digit(s[i].w, -1);
        flag = 0;
    }
}

///Get chronometer clock time in tenths of second
int clock_get_time(void){
   int hours = 10 * v[0] + v[1];
   int minutes = 10 * v[2] + v[3];
   int seconds = 10 * v[4] + v[5];
   int tenths = v[6];
   
   return hours * 36000    // Hours to tenths
        + minutes * 600    // Minutes to tenths
        + seconds * 10     // Seconds to tenths
        + tenths;          // Tenths
}

///Set the UI chornometer clock time given time in tenths of second
void clock_set_time(uint64_t time_in_tenths){
    /// Extract hours, minutes, seconds, and tenths
    int hours = time_in_tenths / 36000;     // 1 hour = 36000 tenths
    int remainder = time_in_tenths % 36000;
    int minutes = remainder / 600;        // 1 minute = 600 tenths
    remainder %= 600;
    int seconds = remainder / 10;         // 1 second = 10 tenths
    int tenths = remainder % 10;          // Tenths place
    
    /// Split into individual digits (HH MM SS T)
    v[0] = hours / 10;       // Tens of hours
    v[1] = hours % 10;       // Units of hours
    v[2] = minutes / 10;      // Tens of minutes
    v[3] = minutes % 10;      // Units of minutes
    v[4] = seconds / 10;      // Tens of seconds
    v[5] = seconds % 10;      // Units of seconds
    v[6] = tenths;            // Tenths of a second
}

void print_digit_zero(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ██████╗ ");
    mvwprintw(w,2,1,"██╔═══██╗");
    mvwprintw(w,3,1,"██║  ╔██║");
    mvwprintw(w,4,1,"██╝  ║██║");
    mvwprintw(w,5,1,"╚██████╔╝");
    mvwprintw(w,6,1," ╚═════╝ ");
    wrefresh(w);
}
void print_digit_one(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1,"    ██╗  ");
    mvwprintw(w,2,1,"   ███║  ");
    mvwprintw(w,3,1,"   ╚██║  ");
    mvwprintw(w,4,1,"    ██║  ");
    mvwprintw(w,5,1,"    ██║  ");
    mvwprintw(w,6,1,"    ╚═╝  ");
    wrefresh(w);
}
void print_digit_two(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ██████╗ ");
    mvwprintw(w,2,1," ╚════██╗");
    mvwprintw(w,3,1,"  █████╔╝");
    mvwprintw(w,4,1," ██╔═══╝ ");
    mvwprintw(w,5,1," ███████╗");
    mvwprintw(w,6,1," ╚══════╝");
    wrefresh(w);
}
void print_digit_three(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ██████╗ ");
    mvwprintw(w,2,1," ╚════██╗");
    mvwprintw(w,3,1,"  █████╔╝");
    mvwprintw(w,4,1,"  ╚═══██╗");
    mvwprintw(w,5,1," ██████╔╝");
    mvwprintw(w,6,1," ╚═════╝ ");
    wrefresh(w);
}
void print_digit_four(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ██╗  ██╗");
    mvwprintw(w,2,1," ██║  ██║");
    mvwprintw(w,3,1," ███████║");
    mvwprintw(w,4,1," ╚════██║");
    mvwprintw(w,5,1,"      ██║");
    mvwprintw(w,6,1,"      ╚═╝");
    wrefresh(w);
}
void print_digit_five(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ███████╗");
    mvwprintw(w,2,1," ██╔════╝");
    mvwprintw(w,3,1," ███████╗");
    mvwprintw(w,4,1," ╚════██║");
    mvwprintw(w,5,1," ███████║");
    mvwprintw(w,6,1," ╚══════╝");
    wrefresh(w);
}

void print_digit_six(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ██████╗ ");
    mvwprintw(w,2,1,"██╔════╝ ");
    mvwprintw(w,3,1,"███████╗ ");
    mvwprintw(w,4,1,"██╔═══██╗");
    mvwprintw(w,5,1,"╚██████╔╝");
    mvwprintw(w,6,1," ╚═════╝ ");
    wrefresh(w);
}
void print_digit_seven(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1," ███████╗");
    mvwprintw(w,2,1," ╚════██║");
    mvwprintw(w,3,1,"     ██╔╝");
    mvwprintw(w,4,1,"    ██║  ");
    mvwprintw(w,5,1,"    ██║  ");
    mvwprintw(w,6,1,"    ╚═╝  ");
    wrefresh(w);
}
void print_digit_eight(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1,"  █████╗ ");
    mvwprintw(w,2,1," ██╔══██╗");
    mvwprintw(w,3,1," ╚█████╔╝");
    mvwprintw(w,4,1," ██╔══██╗");
    mvwprintw(w,5,1," ╚█████╔╝");
    mvwprintw(w,6,1,"  ╚════╝ ");
    wrefresh(w);
}
void print_digit_nine(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1,"  █████╗ ");
    mvwprintw(w,2,1," ██╔══██╗");
    mvwprintw(w,3,1," ╚██████║");
    mvwprintw(w,4,1,"  ╚═══██║");
    mvwprintw(w,5,1,"  █████╔╝");
    mvwprintw(w,6,1,"  ╚════╝ ");
    wrefresh(w);
}
void print_digit_colon(WINDOW* w){
    werase(w);
    box(w,0,0);
    mvwprintw(w,1,1,"   ");
    mvwprintw(w,2,1,"   ");
    mvwprintw(w,3,1,"██╗");
    mvwprintw(w,4,1,"╚═╝");
    mvwprintw(w,5,1,"██╗");
    mvwprintw(w,6,1,"╚═╝");
    wrefresh(w);
}


void print_digit(WINDOW* w, int n) {
    switch (n) {
        case -1: print_digit_colon(w); break;
        case 0:  print_digit_zero(w); break;
        case 1:  print_digit_one(w); break;
        case 2:  print_digit_two(w); break;
        case 3:  print_digit_three(w); break;
        case 4:  print_digit_four(w); break;
        case 5:  print_digit_five(w); break;
        case 6:  print_digit_six(w); break;
        case 7:  print_digit_seven(w); break;
        case 8:  print_digit_eight(w); break;
        case 9:  print_digit_nine(w); break;
        default: break;
    }
}
