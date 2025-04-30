#include <sys/time.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

const int REASONABLE_LEN = 20; // :)

///Get time from EPOCH in tenths
int64_t util_get_current_time_in_tenths(void) {
    struct timeval t;
    gettimeofday(&t, NULL);

    return (int64_t)t.tv_sec * 10
         + (int64_t)(t.tv_usec / 100000);
}

///Save the time studied
void util_save_activity(int total_tenths, char* buff) {
    ///1. get current date in format gg/mm/aa
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char data[REASONABLE_LEN]; // 8 chars + '\0'
    strftime(data, sizeof(data), "%d/%m/%y", tm);
    
    ///2. Eval time in hh:mm:ss
    int hours = total_tenths / 36000;
    total_tenths %= 36000;
    int minutes = total_tenths / 600;
    total_tenths %= 600;
    int seconds = total_tenths / 10;
    
    char clock[REASONABLE_LEN]; // hh:mm:ss + '\0'
    snprintf(clock, sizeof(clock), "%02d:%02d:%02d", hours, minutes, seconds);
    
    ///3. Write into the file
    FILE *fp = fopen("data.txt", "a");
    if (fp == NULL) {
        fprintf(stderr, "Errore apertura file");
        return;
    }
    
    fprintf(fp, "DATA: %s | TIME: %s | LABEL: %s\n", data,clock,buff);
    fclose(fp);
}
