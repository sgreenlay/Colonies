
#ifndef _SHIP_H_
#define _SHIP_H_

#include "engine.h"
#include "graphics.h"

typedef enum {
    ship_type_colony,
    ship_type_carrier,
    ship_type_frigate
} ship_type;

typedef struct _ship {
    float x, y;
    float dx, dy;
    float dest_x, dest_y;
    ship_type type;
    
// private
    sprite m_sprite;
    int m_event;
} ship;


//
// Constructor
//

ship * create_ship();


//
// Methods
//

int ship_init(ship * sh, game * gm, int x, int y, ship_type type);
int ship_update(ship * sh, game * gm, float elapsed);
int ship_fly_to(ship * sh, int x, int y, int event);
int ship_draw(ship * sh, graphics * g);
int ship_cleanup(ship * sh);

#endif