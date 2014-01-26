
#include "ship.h"

#include "sprite_mapping.h"

#include "game.h"

//
// Constructor
//

ship * create_ship()
{
    ship * sh = (ship *)malloc(sizeof(ship));
    
    if (sh == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate ship\n");
        return NULL;
    }
    
    return sh;
}


//
// Methods
//

int ship_init(ship * sh, game * gm, int x, int y, ship_type type)
{
    sh->type = type;
    sh->x = x;
    sh->y = y;
    
    sprite_mapping spm;
    
    switch (type)
    {
        case ship_type_colony:
            spm = get_sprite_mapping(sprite_map_colony_ship);
            break;
        case ship_type_carrier:
            spm = get_sprite_mapping(sprite_map_carrier);
            break;
        case ship_type_frigate:
            spm = get_sprite_mapping(sprite_map_frigate);
            break;
        default:
            ENGINE_DEBUG_LOG_ERROR("ERROR: Unknown ship type [%d]\n", type);
            return 1;
    }
    
    if (sprite_init_from_sheet(&sh->m_sprite, &gm->m_sprites, spm.x, spm.y, spm.w, spm.h))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to initialize ship sprite\n");
        return 1;
    }
    
    return 0;
}

int ship_update(ship * sh, game * gm, float elapsed)
{
    sh->x += 20.0f * elapsed;
    
    return 0;
}

int ship_draw(ship * sh, graphics * g)
{
    if (sprite_draw(&sh->m_sprite, g, sh->x, sh->y))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to draw ship sprite\n");
        return 1;
    }
    
    return 0;
}

int ship_cleanup(ship * sh)
{
    if (sprite_cleanup(&sh->m_sprite))
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to clean-up ship sprite\n");
        return 1;
    }
    
    return 0;
}
