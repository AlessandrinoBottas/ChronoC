#include "chronometer_engine.h"
#include "utils.h"
#include <stdint.h>

///Init the chronometer by filling up the struct ChronoStatus
void chrono_init(struct ChronoStatus* cs) {
    cs->chronoValue = 0;
    cs->pauseValue = 0;
    cs->flagPause = 0;
    cs->timeStart = util_get_current_time_in_tenths();
}

///Update the chronoValue struct member. Needed before every update actions
void chrono_update(struct ChronoStatus* cs){
    if(!cs->flagPause){
        uint64_t timeCurrent = util_get_current_time_in_tenths();
        cs->chronoValue      = timeCurrent - cs->timeStart - cs->pauseValue;
    }
}

///Like init
void chrono_erase(struct ChronoStatus* cs) {
    chrono_init(cs);
}

///Get the curret chonometer clock value (rembebr to call update first)
uint64_t chrono_get_chrono_value(struct ChronoStatus* cs){
    return cs->chronoValue;
}

void chrono_toggle_pause(struct ChronoStatus* cs) {
    if(cs->flagPause == 0) cs->t = util_get_current_time_in_tenths();
    if(cs->flagPause == 1) cs->pauseValue += util_get_current_time_in_tenths() - cs->t;
    
    cs->flagPause = !cs->flagPause;
}

