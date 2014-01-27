
#include "planet.h"

#include "sprite_mapping.h"
#include "game.h"

//
// Constructor
//

planet * create_planet()
{
    planet * pl = (planet *)malloc(sizeof(planet));
    
    if (pl == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate planet\n");
        return NULL;
    }
    
    return pl;
}


//
// Methods
//

int planet_init(planet * pl, game * gm, int x, int y,int w, int h, planet_type type)
{
    pl->type = type;
    pl->x = x;
    pl->y = y;
	pl->w = w;
	pl->h = h;
    
    sprite_mapping spm;
    
    switch (type)
    {
        case planet_type_human:
            spm = get_sprite_mapping(sprite_map_human_planet);
            break;
        case planet_type_alien:
            spm = get_sprite_mapping(sprite_map_alien_planet);
            break;
        default:
            ENGINE_DEBUG_LOG_ERROR("ERROR: Unknown planet type [%d]\n", type);
            return 1;
    }
    
    if (sprite_init_from_sheet(&pl->m_sprite, &gm->m_sprites, spm.x, spm.y, spm.w, spm.h))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize planet sprite\n");
        return 1;
    }
    
    return 0;
}

int planet_draw(planet * pl, graphics * g)
{
    if (sprite_draw(&pl->m_sprite, g, pl->x, pl->y))
    {
		//TODO: shrink sprite
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw planet sprite\n");
        return 1;
    }
    
    return 0;
}

int planet_cleanup(planet * pl)
{
    if (sprite_cleanup(&pl->m_sprite))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up planet sprite\n");
        return 1;
    }
    
    return 0;
}
