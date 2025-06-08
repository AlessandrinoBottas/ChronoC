#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include <curses.h>
#include <stdint.h>

int     display_setup(WINDOW* wmain);                   // Setup all the user interface and graphics
void    display_clock_refresh(uint64_t chronoValue);    // Update the digits on the clock

///Window Label Functions
int     display_init_window_label();                    // Setup Window Label
int display_handle_label_input(void);
char* display_get_label();

///Graph Label Functions
int     display_init_window_graph();
int display_handle_graph_input(void);

#endif
