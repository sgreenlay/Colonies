
#ifndef _PLANET_H_
#define _PLANET_H_

#include "engine.h"
#include "graphics.h"

typedef enum {
    planet_type_human,
    planet_type_alien
} planet_type;

typedef struct _planet {
    int x, y, w, h;
    planet_type type;
    
// private
    sprite m_sprite;
} planet;


//
// Constructor
//

planet * create_planet();


//
// Methods
//

int planet_init(planet * pl, game * gm, int x, int y, planet_type type);
int planet_draw(planet * pl, graphics * g);
int planet_cleanup(planet * pl);

#endif
