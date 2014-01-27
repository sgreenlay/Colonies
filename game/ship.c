
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
    sh->dest_x = sh->x = x;
    sh->dest_y = sh->y = y;
    
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

int ship_fly_to(ship * sh, int x, int y, int event)
{
    sh->dest_x = x;
    sh->dest_y = y;
    
    if ((sh->x != sh->dest_x) || (sh->y != sh->dest_y))
    {
        float disp_x = sh->dest_x - sh->x;
        float disp_y = sh->dest_y - sh->y;
        
        float disp_len = abs(disp_x) + abs(disp_y);
        
        if (disp_len > 0)
        {
            sh->dx = disp_x / disp_len;
            sh->dy = disp_y / disp_len;
            
            sh->m_event = event;
        }
        else
        {
            sh->dx = 0;
            sh->dy = 0;
            
            return event;
        }
    }
    else
    {
        sh->dx = 0;
        sh->dy = 0;
        
        return event;
    }
    
    return 0;
}

int ship_update(ship * sh, game * gm, float elapsed)
{
    if ((sh->x != sh->dest_x) || (sh->y != sh->dest_y))
    {
        sh->x += 20.0f * sh->dx * elapsed;
        
        if (((sh->dx < 0) && (sh->x <= sh->dest_x)) ||
            ((sh->dx > 0) && (sh->x >= sh->dest_x)))
        {
            sh->x = sh->dest_x;
            sh->dx = 0;
        }
        
        sh->y += 20.0f * sh->dy * elapsed;
        
        if (((sh->dy < 0) && (sh->y <= sh->dest_y)) ||
            ((sh->dy > 0) && (sh->y >= sh->dest_y)))
        {
            sh->y = sh->dest_y;
            sh->dy = 0;
        }
    }
    else if (sh->m_event)
    {
        int event = sh->m_event;
        sh->m_event = 0;
        return event;
    }
    
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
