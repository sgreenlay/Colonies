
#include "game.h"

#include "sprite_mapping.h"

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
    
    if (font_sheet_init(&gm->m_font, g, "assets/font_tilemap.png"))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize font sheet\n");
        return 1;
    }

    gm->m_planet_count = 4;
    for (idx = 0; idx < gm->m_planet_count; idx++)
    {
        planet_type type;
        int x, y;
        int w, h;
        
        switch (idx)
        {
            case 0:
                type = planet_type_human;
                
                x = g->width / 5;
                y = 2 * g->height / 3;
                w = 128;
                h = 128;
                
                break;
            case 1:
                type = planet_type_alien;
                
                x = 4 * g->width / 5;
                y = g->height / 3;
                w = 128;
                h = 128;
                
                break;
            case 2:
                type = planet_type_human;
                
                x = 2 * g->width / 6;
                y = g->height / 4;
                w = 64;
                h = 64;
                
                break;
            case 3:
                type = planet_type_alien;
                
                x = 4 * g->width / 6;
                y = 3 * g->height / 4;
                w = 64;
                h = 64;
                
                break;
            default:
                break;
        }
        
        if (planet_init(&gm->m_planets[idx], gm, x, y, w, h, type))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize planet %d\n", idx);
            return 1;
        }
    }
    
    gm->m_ship_count = 1;
    for (idx = 0; idx < gm->m_ship_count; idx++)
    {
        ship_type type;
        int x, y, w, h;
        
        switch (idx)
        {
            case 0:
                type = ship_type_colony;
                
                x = gm->m_planets[idx].x;
                y = gm->m_planets[idx].y;
                
                w = 64;
                h = 64;
                
                break;
            default:
                break;
        }
        
        if (ship_init(&gm->m_ships[idx], gm, x, y, w, h, type))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize ship %d\n", idx);
            return 1;
        }
    }
    
    sprite_mapping spm = get_sprite_mapping(sprite_map_selection);
    
    if (sprite_init_from_sheet(&gm->m_selection_sprite, &gm->m_sprites, spm.x, spm.y, spm.w, spm.h))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize cursor sprite\n");
        return 1;
    }
    
    spm = get_sprite_mapping(sprite_map_cursor);
    
    if (sprite_init_from_sheet(&gm->m_cursor_sprite, &gm->m_sprites, spm.x, spm.y, spm.w, spm.h))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize cursor sprite\n");
        return 1;
    }
    
    return 0;
}

hit_test_target game_hit_test(game * gm, int x, int y)
{
    int idx = 0;
    
    hit_test_target target = { hit_test_target_none, 0 };
    
    for (idx = 0; idx < gm->m_ship_count; idx++)
    {
        if (ship_hit_test(&gm->m_ships[idx], x, y))
        {
            target.type = hit_test_target_ship;
            target.index = idx;
            
            return target;
        }
    }
    
    for (idx = 0; idx < gm->m_planet_count; idx++)
    {
        if (planet_hit_test(&gm->m_planets[idx], x, y))
        {
            target.type = hit_test_target_planet;
            target.index = idx;
            
            return target;
        }
    }
    
    return target;
}

int game_update(game * gm, engine * e, unsigned int dt)
{
    int idx = 0;
    
    input * i = engine_get_input(e);
    
    int is_mouse_left_down = 0;
    int is_mouse_right_down = 0;
    
    if (input_get_mouse_state(i, &is_mouse_left_down, &is_mouse_right_down, &gm->m_cursor_x, &gm->m_cursor_y))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to get mouse state\n");
        return 1;
    }
    
    if (is_mouse_left_down)
    {
        hit_test_target target = game_hit_test(gm, gm->m_cursor_x, gm->m_cursor_y);
        
        if ((target.type != gm->m_current_target.type) || 
            (target.index != gm->m_current_target.index))
        {
            gm->m_current_target = target;
        }
    }
    else if (is_mouse_right_down)
    {
        if (gm->m_current_target.type == hit_test_target_ship)
        {
            hit_test_target target = game_hit_test(gm, gm->m_cursor_x, gm->m_cursor_y);
            
            if (target.type == hit_test_target_planet)
            {
                ship * ship = &gm->m_ships[gm->m_current_target.index];
                planet * planet = &gm->m_planets[target.index];
                
                if (ship_fly_to(ship, planet->x, planet->y, target.index + 1) != 0)
                {
                    ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to set ship destination %d\n", gm->m_current_target.index);
                    return 1;
                }
            }
        }
    }
    
    float elapsed = (float)dt / 1000;
    
    for (idx = 0; idx < 1; idx++)
    {
        int evt = ship_update(&gm->m_ships[idx], gm, elapsed);
        
        if (evt != 0)
        {
            planet * planet = &gm->m_planets[evt - 1];
            
            if (planet->type != planet_type_human)
            {
                planet_set_type(planet, gm, planet_type_human);
            }
            
            ENGINE_DEBUG_LOG_ERROR("Ship %d arrived at planet %d\n", idx, evt - 1);
        }
    }
    
    return 0;
}

int game_render(game * gm, graphics * g)
{
    int idx = 0;
	SDL_Color BLACK_COLOR = { 255, 0, 0, 0 };

    for (idx = 0; idx < gm->m_planet_count; idx++)
    {
        if (planet_draw(&gm->m_planets[idx], g))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw planet %d\n", idx);
            return 1;
        }
        
        if ((gm->m_current_target.type == hit_test_target_planet) &&
            (gm->m_current_target.index == idx))
        {
            if (sprite_draw_scaled(&gm->m_selection_sprite, g,
                gm->m_planets[idx].x, gm->m_planets[idx].y,
                gm->m_planets[idx].w, gm->m_planets[idx].h))
            {
                ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw selection sprite\n");
                return 1;
            }
        }
    }
    
    for (idx = 0; idx < gm->m_ship_count; idx++)
    {
        if (ship_draw(&gm->m_ships[idx], g))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw ship %d\n", idx);
            return 1;
        }
        
        if ((gm->m_current_target.type == hit_test_target_ship) &&
            (gm->m_current_target.index == idx))
        {
            if (sprite_draw_scaled(&gm->m_selection_sprite, g,
                gm->m_ships[idx].x, gm->m_ships[idx].y,
                gm->m_ships[idx].w, gm->m_ships[idx].h))
            {
                ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw selection sprite\n");
                return 1;
            }
        }
    }

	drawString(&gm->m_font, g, "Human Power", 300, 400, 30, &BLACK_COLOR);

    if (sprite_draw(&gm->m_cursor_sprite, g, gm->m_cursor_x, gm->m_cursor_y))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw cursor sprite\n");
        return 1;
    }
    
    return 0;
}

int game_cleanup(game * gm)
{
    int idx = 0;
    
    if (sprite_cleanup(&gm->m_selection_sprite))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up selection sprite\n");
        return 1;
    }
    
    if (sprite_cleanup(&gm->m_cursor_sprite))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up cursor sprite\n");
        return 1;
    }
    
    for (idx = 0; idx < gm->m_ship_count; idx++)
    {
        if (ship_cleanup(&gm->m_ships[idx]))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up ship %d\n", idx);
            return 1;
        }
    }
    
    for (idx = 0; idx < gm->m_planet_count; idx++)
    {
        if (planet_cleanup(&gm->m_planets[idx]))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up planet %d\n", idx);
            return 1;
        }
    }
    
    return 0;
}
