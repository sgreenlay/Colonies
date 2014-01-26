
#include "engine.h"

#include "game.h"


//
// Constructor
//

engine * create_engine()
{
    engine * e = (engine *)malloc(sizeof(engine));
    
    if (e == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate engine\n");
        return NULL;
    }
    
    return e;
}


//
// Accessors
//

graphics * engine_get_graphics(engine * e)
{
    return &(e->m_graphics);
}

input * engine_get_input(engine * e)
{
    return &(e->m_input);
}


//
// Methods
//

int engine_init(engine * e, int width, int height)
{
    if (graphics_init(&e->m_graphics, width, height))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize graphics engine\n");
        return 1;
    }
    
    if (input_init(&e->m_input))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize input\n");
        return 1;
    }
    
    if (clock_init(&e->m_clock))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize clock\n");
        return 1;
    }
    
    return 0;
}

int engine_cleanup(engine * e)
{
    if (clock_cleanup(&e->m_clock))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up clock\n");
        return 1;
    }
    
    if (input_cleanup(&e->m_input))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up input\n");
        return 1;
    }
    
    if (graphics_cleanup(&e->m_graphics))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up graphics engine\n");
        return 1;
    }
    
    return 0;
}

void engine_run(engine * e, game * gm)
{
    while (1)
    {
        unsigned int dt = clock_tick(&e->m_clock);
        
        if (input_poll(&e->m_input))
        {
            return;
        }
        
        game_update(gm, e, dt);
        
        if (graphics_draw(&e->m_graphics, gm))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Drawing failed\n");
            return;
        }
    }
}

