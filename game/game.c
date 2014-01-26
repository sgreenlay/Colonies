
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
    
    if (sprite_sheet_init(&gm->m_sprites, g, 50, 50, "assets/sprites.bmp"))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize sprite sheet\n");
        return 1;
    }
    
    if (sprite_init_from_sheet(&gm->m_mouse_sprite, &gm->m_sprites, 50, 0, 25, 25))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize mouse sprite\n");
        return 1;
    }
    
    gm->m_character_x = g->width / 2 - 25;
    gm->m_character_y = g->height / 2 - 25;
    
    if (sprite_init_from_sheet(&gm->m_character_sprite, &gm->m_sprites, 0, 0, 50, 50))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize character sprite\n");
        return 1;
    }
    
    if (sprite_init_from_sheet(&gm->m_object_sprite, &gm->m_sprites, 0, 50, 150, 150))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize object sprite\n");
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
    
    int mouse_down = 0;
    
    if (input_get_mouse_state(i, &mouse_down, &gm->m_mouse_x, &gm->m_mouse_y))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to get mouse state\n");
        return 1;
    }
    
    return 0;
}

int game_render(game * gm, graphics * g)
{
    if (sprite_draw(&gm->m_object_sprite, g, 20, 20))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw object sprite\n");
        return 1;
    }
    
    int x = (int)gm->m_character_x;
    int y = (int)gm->m_character_y;
    
    if (sprite_draw(&gm->m_character_sprite, g, x, y))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw character sprite\n");
        return 1;
    }
    
    if (sprite_draw(&gm->m_mouse_sprite, g, gm->m_mouse_x - 25, gm->m_mouse_y - 25))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw mouse sprite\n");
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
