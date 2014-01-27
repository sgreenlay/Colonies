
#include "game.h"

#include "sprite_mapping.h"
#include "credits.h"
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
    
	gm->game_status = MENU_CREDITS;

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

int game_update_ships(game * gm, engine * e, int is_mouse_left_down, int is_mouse_right_down, unsigned int dt)
{
	int idx = 0;
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

		if (evt > 0)
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

	switch (gm->game_status)
	{
	case MENU_CREDITS: 
		if (input_is_key_down(&e->m_input,Key_Space, 0))
			gm->game_status = MENU_MAIN;
		break;
	case MENU_MAIN: //TODO: sigh
		if (input_is_key_down(&e->m_input, Key_Char, 's'))
			gm->game_status = MENU_GAME;
		if (input_is_key_down(&e->m_input, Key_Char, 'c'))
			gm->game_status = MENU_CREDITS;
		if (input_is_key_down(&e->m_input, Key_Char, 'q'))
			gm->game_status = MENU_QUIT;
		break;
	case MENU_GAME:
		if (game_update_ships(gm, e, is_mouse_left_down, is_mouse_right_down, dt) != 0)
			return 1;
		if (input_is_key_down(&e->m_input, Key_Char, 'p'))
			gm->game_status = MENU_PAUSE;
		break; //GAME
	case MENU_PAUSE: 
		if (input_is_key_down(&e->m_input, Key_Space, 0))
			gm->game_status = MENU_GAME;
		break;
	case MENU_WIN:  //FALL-THROUGH
	case MENU_LOSE: 
		if (input_is_key_down(&e->m_input, Key_Space, 0))
			gm->game_status = MENU_MAIN;
		break; 
	case MENU_QUIT: return 2;
	default: 
		ENGINE_DEBUG_LOG_ERROR("Error! Unknown state%i\n", gm->game_status);
		return 1;
	}
    
    
    return 0;
}


int game_render_planets(game *gm, graphics* g)
{
	int idx = 0;
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
	return 0;
}

int game_render_ships(game *gm, graphics* g)
{
	int idx = 0;
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
	//this will be part of GUI
	setFontColor(g, 255, 0, 0);
	drawString(&gm->m_font, g, "Human Power", 300, 400, 30);
	return 0;
}

int game_render(game * gm, graphics * g)
{
    int idx = 0;
	char* print_string;

	switch (gm->game_status)
	{
	case MENU_CREDITS: 
		setFontColor(g, 255, 255, 255);
		drawString(&gm->m_font, g, "Press Space to skip.", 80, 20, 30);
		for (idx = 0; idx < sizeof(CREDITS) / sizeof(char*); idx++)
			drawString(&gm->m_font, g, CREDITS[idx], 40, 80 + idx * 30, 25);
		break;
	case MENU_MAIN: 
		setFontColor(g, 255, 255, 255);
		drawString(&gm->m_font, g, "Press letters to select", 20, 20, 20);
		drawString(&gm->m_font, g, "s)Start New Game.", 20, 120, 40);
		drawString(&gm->m_font, g, "c)Credits.", 20,220, 40);
		drawString(&gm->m_font, g, "q)Quit.", 20, 320, 40);
		break;
	case MENU_GAME: 
		game_render_planets(gm, g);
		game_render_ships(gm, g); 
		//TODO: GUI
		break;
	case MENU_PAUSE: 
		game_render_planets(gm, g);
		game_render_ships(gm, g); 
		setFontColor(g, 255, 255, 255);
		drawString(&gm->m_font, g, "PAUSED.", 300, 480 / 2, 30);
		drawString(&gm->m_font, g, "Press space to unpause.", 100, 480 / 2+200, 20);
		break;
	case MENU_WIN:  //FALL-THROUGH CASE
	case MENU_LOSE: 
		game_render_planets(gm, g);
		game_render_ships(gm, g);
		setFontColor(g, 255, 0, 0);
		if (gm->game_status == MENU_WIN)
			print_string = "YOU WIN!!!";
		else
			print_string = "YOU LOSE!!!";
		drawString(&gm->m_font, g, print_string, 800 / 2 - 100, 480 / 2 , 30);
		break;
	case MENU_QUIT: break; //we will go away soon, just quit.
		ENGINE_DEBUG_LOG_ERROR("Error! Unknown state%i\n", gm->game_status);
		return 1;
	}


	

	//always draw cursor
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
