#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include <curses.h>
#include <stdint.h>

int     display_setup(WINDOW* wmain);   //setup all the user interface and graphics
void    display_clock_refresh(uint64_t chronoValue);   //update the digits on the clock
int     display_init_window_label();
void    display_delete_window_label();
void    display_set_window_clock_name(char *s);
void    display_update_label_input_text(char* label);
WINDOW* display_get_window_label(void);
#endif
