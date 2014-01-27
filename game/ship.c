
#include "ship.h"

#include <math.h>

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

int ship_init(ship * sh, game * gm, int x, int y,int w,int h, ship_type type)
{
    sh->type = type;
    sh->dest_x = sh->x = x;
    sh->dest_y = sh->y = y;
    sh->dest_r = sh->r = 0;
    
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
    
    sh->w = spm.w;
    sh->h = spm.h;
    
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
            
            sh->dest_r = atan2(sh->dy, sh->dx) * 180.0f / M_PI;
            
            if (sh->dest_r > 360.0f)
            {
                sh->dest_r -= 360.0f;
            }
            else if (sh->dest_r < 0.0f)
            {
                sh->dest_r += 360.0f;
            }
            
            sh->m_event = event;
        }
        else
        {
            sh->dx = 0.0f;
            sh->dy = 0.0f;
        }
    }
    else
    {
        sh->dx = 0.0f;
        sh->dy = 0.0f;
    }
    
    if (sh->dest_r != sh->r)
    {
        float ccw = 0.0f;
        float cw = 0.0f;
        
        if (sh->r < sh->dest_r)
        {
            ccw = sh->dest_r - sh->r;
            cw = 360 - ccw;
        }
        else
        {
            cw = sh->r - sh->dest_r;
            ccw = 360 - cw;
        }
        
        if (ccw > cw)
        {
            sh->dr = -1.0f;
        }
        else
        {
            sh->dr = 1.0f;
        }
    }
    
    return 0;
}

int ship_update(ship * sh, game * gm, float elapsed)
{
    if (sh->dr != 0)
    {
        float dr = 90.0f * sh->dr * elapsed;
        
        if (((sh->r < sh->dest_r) && (sh->r + dr > sh->dest_r)) ||
            ((sh->r > sh->dest_r) && (sh->r + dr < sh->dest_r)))
        {
            sh->r = sh->dest_r;
            sh->dr = 0;
        }
        else
        {
            sh->r += dr;
        }
        
        if (sh->r > 360.0f)
        {
            sh->r -= 360.0f;
        }
        else if (sh->r < 0.0f)
        {
            sh->r += 360.0f;
        }
    }
    else if ((sh->dx != 0) || (sh->dy != 0))
    {
        sh->x += 30.0f * sh->dx * elapsed;
        
        if (((sh->dx < 0) && (sh->x <= sh->dest_x)) ||
            ((sh->dx > 0) && (sh->x >= sh->dest_x)))
        {
            sh->x = sh->dest_x;
            sh->dx = 0;
        }
        
        sh->y += 30.0f * sh->dy * elapsed;
        
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

int ship_hit_test(ship * sh, int x, int y)
{
    if ((x >= sh->x - sh->w / 2) && (x <= sh->x + sh->w / 2) &&
        (y >= sh->y - sh->h / 2) && (y <= sh->y + sh->h / 2))
    {
        return 1;
    }
    
    return 0;
}

int ship_draw(ship * sh, graphics * g)
{
    if (sprite_draw_scaled_and_rotated(&sh->m_sprite, g, sh->x, sh->y, sh->w, sh->h, sh->r))
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
