#ifndef CLOCK_H
#define CLOCK_H

#include <curses.h>

#define LABEL_LEN 50

extern const int CLOCK_WIDTH;
extern const int CLOCK_HEIGTH;

struct WindowClock{
    WINDOW* wclock;
    int x;
    int y;
    int nlines;
    int ncols;
    int xMid;
    int yMid;
};

int clock_windows_init(struct WindowClock* wc); //Create the windows for digits and separators from a starting position
void clock_update(void); //Refresh digits
void clock_increment_tenths(void);
void clock_set_time(uint64_t time_in_tenths); //Set the UI chornometer clock time given time in tenths of second
int  clock_get_time(void); //Get UI chronometer clock time in tenths of second

#endif
