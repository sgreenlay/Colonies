
#include "clock.h"


//
// Constructor
//

clock * create_clock()
{
    clock * c = (clock *)malloc(sizeof(clock));
    
    if (c == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate clock\n");
        return NULL;
    }
    
    return c;
}


//
// Methods
//

int clock_init(clock * c)
{
    c->m_last_tick = SDL_GetTicks();
    
    return 0;
}

unsigned int clock_tick(clock * c)
{
    unsigned int current_ticks = SDL_GetTicks();
    unsigned int dt = current_ticks - c->m_last_tick;
    c->m_last_tick = current_ticks;
    
    return dt;
}

int clock_cleanup(clock * c)
{
    // Do nothing.
    
    return 0;
}
