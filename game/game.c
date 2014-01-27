
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
	if (is_mouse_left_down == 1)
	{
		//TEMP: TODO: REMOVE
		int selected_active = gm->selected_active;
		SelectedObject selected_type = gm->selected_type;
		int selection_id = gm->selection_id;
		//
		int hit_something = 0;
		//first check hit box on planets
		for (idx = 0; idx < 2; idx++)
		{
			if (point_intersect_box(gm->m_cursor_x, gm->m_cursor_y, gm->m_planets[idx].x, gm->m_planets[idx].y, gm->m_planets[idx].w,
				gm->m_planets[idx].h))
			{
				hit_something = 1;
				gm->selected_active = 1;
				gm->selected_type = SOPlanet;
				gm->selection_id = idx;
				break;
			}
		}
		if (hit_something==0) //check for  ships if no planets hit
		for (idx = 0; idx < 1; idx++)
		{
			if (point_intersect_box(gm->m_cursor_x, gm->m_cursor_y, gm->m_ships[idx].x, gm->m_ships[idx].y, gm->m_ships[idx].w,
				gm->m_ships[idx].h))
			{
				hit_something = 1;
				gm->selected_active = 1;
				gm->selected_type = SOShip;
				gm->selection_id = idx;
				break;
			}
		}
		if (hit_something == 0) //did not hit anything
		{
			if (selected_active == 1)
				printf("Unselecting\n");
			gm->selected_active = 0; gm->selected_type = SONothing;
		}
		else //TODO:REMOVE
		{
			if (0 == ((selected_active == gm->selected_active) && (gm->selected_type == selected_type) && (selection_id == gm->selection_id)))
			{
				printf("Hit a new target %i, %i, %i\n", (int)gm->selected_active, (int)gm->selected_type, (int)gm->selection_id);
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
	SDL_Color BLACK_COLOR = { 0, 0, 0, 0 };

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
	if (gm->selected_active == 1)
	{
		int x = 0;
		int y = 0;
		if (gm->selected_type == SOPlanet)
		{
			x = gm->m_planets[gm->selection_id].x;
			y = gm->m_planets[gm->selection_id].y;
		}
		else if (gm->selected_type == SOShip)
		{
			x = gm->m_ships[gm->selection_id].x;
			y = gm->m_ships[gm->selection_id].y;
		}
		else
		{
			ENGINE_DEBUG_LOG_ERROR("ERROR: object selected is of type %d\n", (int) gm->selected_type);
		}

		(void)x; (void)y;
		//SDL_FillRect(g,x....y....);
	}

	drawString(&gm->m_font, g, "hello meow", 5, 5, 10, BLACK_COLOR);

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
