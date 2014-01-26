
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
        planet_type type;
        int x;
        int y = g->height / 2;
        
        switch (idx)
        {
            case 0:
                type = planet_type_human;
                x = g->width / 4;
                break;
            case 1:
                type = planet_type_alien;
                x = 3 * g->width / 4;
                break;
            default:
                break;
        }
        
        if (planet_init(&gm->m_planets[idx], gm, x, y, type))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize planet %d\n", idx);
            return 1;
        }
    }
    
    for (idx = 0; idx < 1; idx++)
    {
        ship_type type;
        int x;
        int y = g->height / 2;
        
        switch (idx)
        {
            case 0:
                type = ship_type_colony;
                x = g->width / 4;
                break;
            default:
                break;
        }
        
        if (ship_init(&gm->m_ships[idx], gm, x, y, type))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize ship %d\n", idx);
            return 1;
        }
    }
    
    return 0;
}

int game_update(game * gm, engine * e, unsigned int dt)
{
    int idx = 0;
    
    input * i = engine_get_input(e);
    
    float elapsed = (float)dt / 1000;
    
    for (idx = 0; idx < 1; idx++)
    {
        if (ship_update(&gm->m_ships[idx], gm, elapsed))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to update ship %d\n", idx);
            return 1;
        }
    }
    
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
    
    for (idx = 0; idx < 1; idx++)
    {
        if (ship_draw(&gm->m_ships[idx], g))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw ship %d\n", idx);
            return 1;
        }
    }
    
    return 0;
}

int game_cleanup(game * gm)
{
    int idx = 0;
    
    for (idx = 0; idx < 1; idx++)
    {
        if (ship_cleanup(&gm->m_ships[idx]))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up ship %d\n", idx);
            return 1;
        }
    }
    
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
