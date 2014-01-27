
#ifndef _PLANET_H_
#define _PLANET_H_

#include "engine.h"
#include "graphics.h"

typedef enum {
    planet_type_none,
    planet_type_human,
    planet_type_alien
} planet_type;

typedef struct _planet {
    int x, y;
	int w, h;
    planet_type type;
    
// private
    sprite m_sprite;
} planet;


//
// Constructor
//

planet * create_planet();


//
// Setters
//

int planet_set_type(planet * pl, game * gm, planet_type type);


//
// Methods
//

int planet_init(planet * pl, game * gm, int x, int y, int w, int h, planet_type type);
int planet_draw(planet * pl, graphics * g);
int planet_hit_test(planet * pl, int x, int y);
int planet_cleanup(planet * pl);

#endif
