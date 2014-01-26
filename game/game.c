
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
    graphics * g = engine_get_graphics(e);
    
    gm->m_character_x = g->width / 2 - 25;
    gm->m_character_y = g->height / 2 - 25;
    
    if (sprite_init(&gm->m_character_sprite, g, 50, 50, "assets/test.bmp"))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize character sprite\n");
        return 1;
    }
    
    return 0;
}

int game_update(game * gm, engine * e, unsigned int dt)
{
    input * i = engine_get_input(e);
    
    float elapsed = (float)dt / 1000;
    
    if (input_is_key_down(i, Key_Up))
    {
        gm->m_character_y -= 50.0f * elapsed;
    }
    if (input_is_key_down(i, Key_Down))
    {
        gm->m_character_y += 50.0f * elapsed;
    }
    if (input_is_key_down(i, Key_Left))
    {
        gm->m_character_x -= 50.0f * elapsed;
    }
    if (input_is_key_down(i, Key_Right))
    {
        gm->m_character_x += 50.0f * elapsed;
    }
    
    return 0;
}

int game_render(game * gm, graphics * g)
{
    int x = (int)gm->m_character_x;
    int y = (int)gm->m_character_y;
    
    if (sprite_draw(&gm->m_character_sprite, g, x, y))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw character sprite\n");
        return 1;
    }
    
    return 0;
}

int game_cleanup(game * gm)
{
    if (sprite_cleanup(&gm->m_character_sprite))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up character sprite\n");
        return 1;
    }
    
    return 0;
}
