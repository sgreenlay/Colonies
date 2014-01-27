
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
        
		//TODO: replace 128. 
        if (planet_init(&gm->m_planets[idx], gm, x, y,128,128, type))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize planet %d\n", idx);
            return 1;
        }
    }
    
    for (idx = 0; idx < 1; idx++)
    {
        ship_type type;
        
        int x, y;
        int dest_x, dest_y;
        int event;
        
        switch (idx)
        {
            case 0:
                type = ship_type_colony;
                
                x = gm->m_planets[idx].x;
                y = gm->m_planets[idx].y;
                
                dest_x = gm->m_planets[idx + 1].x;
                dest_y = gm->m_planets[idx + 1].y;
                
                event = idx + 2;
                
                break;
            default:
                break;
        }
        //TODO: replace 64,64
        if (ship_init(&gm->m_ships[idx], gm, x, y,64,64, type))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize ship %d\n", idx);
            return 1;
        }
        
        if (ship_fly_to(&gm->m_ships[idx], dest_x, dest_y, event) != 0)
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to set ship destination %d\n", idx);
            return 1;
        }
    }
    
    sprite_mapping spm = get_sprite_mapping(sprite_map_cursor);
    
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
    
    for (idx = 0; idx < 1; idx++)
    {
        if (ship_hit_test(&gm->m_ships[idx], x, y))
        {
            target.type = hit_test_target_ship;
            target.index = idx;
            
            return target;
        }
    }
    
    for (idx = 0; idx < 2; idx++)
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
            ENGINE_DEBUG_LOG("Current target %d %d\n", target.type, target.index);
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
                
                if (ship_fly_to(ship, planet->x, planet->y, target.index) != 0)
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
            int dest_x, dest_y;
            
            switch (evt)
            {
                case 1:
                    dest_x = gm->m_planets[1].x;
                    dest_y = gm->m_planets[1].y;    
                    evt = 2;
                    
                    break;
                case 2:
                    dest_x = gm->m_planets[0].x;
                    dest_y = gm->m_planets[0].y; 
                    evt = 1;
                    
                    break;
                default:
                    break;
            }
            
            if (ship_fly_to(&gm->m_ships[idx], dest_x, dest_y, evt) != 0)
            {
                ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to set ship destination %d\n", idx);
                return 1;
            }
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
    
    if (sprite_cleanup(&gm->m_cursor_sprite))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up cursor sprite\n");
        return 1;
    }
    
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
