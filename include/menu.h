#ifndef MENU_H
#define MENU_H

#include <curses.h>

extern const int MENU_HEIGTH;

struct WindowMenu{
    WINDOW* wmenu;
    int x;
    int y;
    int nlines;
    int ncols;
    int xMid;
    int yMid;
};

struct WindowLabel{
    WINDOW* wlabel;
    int x;
    int y;
    int nlines;
    int ncols;
    int xMid;
    int yMid;
};

struct WindowGraph{
    WINDOW* wgraph;
    int x;
    int y;
    int nlines;
    int ncols;
    int xMid;
    int yMid;
};

void menu_init(struct WindowMenu* wm);  //Init the window of the menu
#endif
