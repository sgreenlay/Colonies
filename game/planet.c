
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
// Setters
//

int planet_set_type(planet * pl, game * gm, planet_type type)
{
    if (type != pl->type)
    {
        if (sprite_cleanup(&pl->m_sprite))
        {
            ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up planet sprite\n");
            return 1;
        }
        
        pl->type = type;
        
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
    }
    
    return 0;
}


//
// Methods
//

int planet_init(planet * pl, game * gm, int x, int y,int w, int h, planet_type type)
{
    pl->type = planet_type_none;
    pl->x = x;
    pl->y = y;
    pl->w = w;
    pl->h = h;
    
    planet_set_type(pl, gm, type);
    
    return 0;
}

int planet_hit_test(planet * pl, int x, int y)
{
    if ((x >= pl->x - pl->w / 2) && (x <= pl->x + pl->w / 2) &&
        (y >= pl->y - pl->h / 2) && (y <= pl->y + pl->h / 2))
    {
        return 1;
    }
    
    return 0;
}

int planet_draw(planet * pl, graphics * g)
{
    if (sprite_draw_scaled(&pl->m_sprite, g, pl->x, pl->y, pl->w, pl->h))
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
