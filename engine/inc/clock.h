
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "util.h"
#include "graphics.h"

typedef struct _clock {
// private
    unsigned int m_last_tick;
} clock;


//
// Constructor
//

clock * create_clock();


//
// Methods
//

int clock_init(clock * c);
unsigned int clock_tick(clock * c);
int clock_cleanup(clock * c);

#endif
