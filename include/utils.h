#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"

uint64_t util_get_current_time_in_tenths(void);         //Get time from EPOCH in tenths
void util_save_activity(int total_tenths, char* buff);  //Save to file
char** util_retrieve_data_for_graph();                  //Read from file

#endif
