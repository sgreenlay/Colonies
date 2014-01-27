
#ifndef _SHIP_H_
#define _SHIP_H_

#include "engine.h"
#include "graphics.h"

typedef enum {
    ship_type_none,
    ship_type_human,
    ship_type_alien
} ship_type;

typedef enum {
    ship_class_colony,
    ship_class_carrier,
    ship_class_frigate
} ship_class;

typedef struct _ship {
    float x, y;
    float r;
    int w, h; 
    float dx, dy;
    float dr;
    float dest_x, dest_y;
    float dest_r;
    ship_type ty;
    ship_class cl;
    
// private
    sprite m_sprite;
    int m_event;
} ship;


//
// Constructor
//

ship * create_ship();


//
// Setters
//

int ship_set_type(ship * sh, game * gm, ship_type type);


//
// Methods
//

int ship_init(ship * sh, game * gm, int x, int y, int w, int h, ship_class cl, ship_type ty);
int ship_update(ship * sh, game * gm, float elapsed);
int ship_fly_to(ship * sh, int x, int y, int event);
int ship_draw(ship * sh, graphics * g);
int ship_hit_test(ship * sh, int x, int y);
int ship_cleanup(ship * sh);

#endif
