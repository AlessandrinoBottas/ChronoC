#include <sys/time.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // getenv
#include <limits.h>   // PATH_MAX
#include <clock.h>    // LABLE_LEN
#include <curses.h>

#define BAR_LEN 34
#define BUF_LEN 1024

char *ERRORS[] = {
    "***COULDN'T OPEN FILE***",
    "***EMPTY FILE or WRONG FORMAT READING/WRITTEN***",
    NULL    //sentinel
};


const int LEN = 100;
char FILE_NAME[] = "Documents/ChronoC_Log.txt";

struct Data {
    char label[LEN];
    uint64_t time;
};

void generate_bar(float percentage, char* bar, size_t bar_length) {
    if (bar_length < 2) { if (bar_length >= 1) bar[0] = '\0'; return; }
    
    int total_slots = (int)(bar_length - 1);  // excluding '\0'
    int filled = ((int)percentage / 3);       // round down to 5% step
    if (filled > total_slots) filled = total_slots;

    for (int i = 0; i < total_slots; i++) bar[i] = (i < filled) ? '#' : ' ';
    bar[total_slots] = '\0';
}


char** util_retrieve_data_for_graph(){
    struct Data* d = NULL;
    uint64_t tot_time = 0;
    char date[LEN];
    char label[LEN];
    char clock[LEN];
    int hours, minutes, seconds;
    int cnt = 1;
    char filepath[PATH_MAX + 64];
    
    ///Getting the home path and then concat with Documents folder
    const char* home = getenv("HOME");
    snprintf(filepath, sizeof(filepath), "%s/%s", home, FILE_NAME);
    
    ///Opening the file in read mode
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) return NULL;
    
    ///Fill struct with first data
    if(fscanf(fp, "DATE: %s | TIME: %s | LABEL: %s\n", date, clock, label) != 3) return NULL;
    d = malloc(sizeof(struct Data) * cnt);
    
    strcpy(d[0].label, label);
    sscanf(clock, "%2d:%2d:%2d", &hours, &minutes, &seconds);
    d[0].time = hours * 3600 + minutes * 60 + seconds;
    
    
    char line[BUF_LEN];
    while(fgets(line, BUF_LEN, fp)){
        if(sscanf(line, "DATE: %s | TIME: %s | LABEL: %[^\n]", date, clock, label)!=3) break;
        
        char found = 0;
        uint64_t t = 0;
        
        sscanf(clock, "%2d:%2d:%2d", &hours, &minutes, &seconds);
        t = hours * 3600 + minutes * 60 + seconds;
        
        for(int i=0; i<cnt; i++){
            if(strcmp(d[i].label, label)) continue;
            ///Se sono qui è perchè ho trovato una corrispondenza e quindi devo incrementare il tempo totale della label
            d[i].time += t;
            found = 1;
        }
        
        if(!found){
            d = realloc(d, sizeof(struct Data) * (++cnt));
            strcpy(d[cnt-1].label, label);
            d[cnt-1].time = t;
        }
    }
    
    fclose(fp);
    
    ///Ora calcolo il tempo totale per fare le percentuali per ogni label
    int magic_number = BUF_LEN;
    char** res = malloc(sizeof(char*) * (cnt + 1));
    res[cnt] = NULL;  //sentinel
    
    for(int i=0; i<cnt; i++){
        tot_time += d[i].time;
        res[i] = malloc(sizeof(char) * magic_number);
    }
    
    for(int i=0; i<cnt; i++){
        float percentage = ((float)d[i].time/(float)tot_time) * 100;
        char bar[BAR_LEN];
        generate_bar(percentage, bar, BAR_LEN);
        int h,m,s;
        uint64_t t2 = d[i].time;
        h = t2 / 3600;
        t2 %= 3600;
        m = t2 / 60;
        t2 %= 60;
        s = t2;
        sprintf(res[i], "%34s  [%s] %05.2f%% - Total time: %02d:%02d:%02d", d[i].label, bar, percentage, h, m, s);
    }
    
    free(d);
    
    return res;
}


///Get time from EPOCH in tenths
int64_t util_get_current_time_in_tenths(void) {
    struct timeval t;
    gettimeofday(&t, NULL);

    return (int64_t)t.tv_sec * 10
         + (int64_t)(t.tv_usec / 100000);
}

///Save the time studied
void util_save_activity(int clock_value, char* buff) {
    /// Get current date in format gg/mm/aa
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char date[LEN]; // 8 chars + '\0'
    strftime(date, sizeof(date), "%d/%m/%y", tm);
    
    /// Eval time in hh:mm:ss
    int hours = clock_value / 36000;
    clock_value %= 36000;
    int minutes = clock_value / 600;
    clock_value %= 600;
    int seconds = clock_value / 10;
    
    char clock[LEN]; // hh:mm:ss + '\0'
    snprintf(clock, sizeof(clock), "%02d:%02d:%02d", hours, minutes, seconds);
    
    const char* home = getenv("HOME");
    char filepath[PATH_MAX + 64];
    snprintf(filepath, sizeof(filepath), "%s/%s", home, FILE_NAME);
    ///Write into the file
    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL) {
        fprintf(stderr, "Errore apertura file");
        return;
    }
    
    fprintf(fp, "DATE: %s | TIME: %s | LABEL: %s\n", date,clock,buff);
    fclose(fp);
}
