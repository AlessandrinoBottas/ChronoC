#ifndef CHRONOMETER_ENGINE_H
#define CHRONOMETER_ENGINE_H

#include "stdint.h"

struct ChronoStatus{
    int flagPause;
    uint64_t chronoValue;
    uint64_t pauseValue;
    uint64_t timeStart;
    uint64_t t;
};

void chrono_init(struct ChronoStatus* cs);//Init the chronometer by filling up the struct ChronoStatus
void chrono_update(struct ChronoStatus* cs); //Update the chronoValue struct member. Needed before every display update actions
void chrono_toggle_pause(struct ChronoStatus* cs);
void chrono_erase(struct ChronoStatus* cs);//Like init

uint64_t chrono_get_chrono_value(struct ChronoStatus* cs); //Get the curret chonometer clock value (rembebr to call update first)

#endif
