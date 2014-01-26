
#include "game.h"


//
// Constructor
//

game * create_game()
{
    game * gm = (game *)malloc(sizeof(game));
    
    if (gm == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate game\n");
        return NULL;
    }
    
    return gm;
}


//
// Methods
//

int game_init(game * gm, engine * e)
{
    int idx = 0;
    
    graphics * g = engine_get_graphics(e);
    
    if (sprite_sheet_init(&gm->m_sprites, g, 50, 50, "assets/sprites.png"))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize sprite sheet\n");
        return 1;
    }
    
    for (idx = 0; idx < 2; idx++)
    {
        if (planet_init(&gm->m_planets[idx], gm, 200 * idx, 200 * idx, (idx % 2 == 0) ? planet_type_human : planet_type_alien))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize planet %d\n", idx);
            return 1;
        }
    }
    
    return 0;
}

int game_update(game * gm, engine * e, unsigned int dt)
{
    input * i = engine_get_input(e);
    
    float elapsed = (float)dt / 1000;
    
    // TODO
    
    return 0;
}

int game_render(game * gm, graphics * g)
{
    int idx = 0;
    
    for (idx = 0; idx < 2; idx++)
    {
        if (planet_draw(&gm->m_planets[idx], g))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw planet %d\n", idx);
            return 1;
        }
    }
    
    return 0;
}

int game_cleanup(game * gm)
{
    int idx = 0;
    
    for (idx = 0; idx < 2; idx++)
    {
        if (planet_cleanup(&gm->m_planets[idx]))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up planet %d\n", idx);
            return 1;
        }
    }
    
    return 0;
}
