
#ifndef _GAME_H_
#define _GAME_H_

#include "engine.h"
#include "graphics.h"
#include "input.h"

#include "planet.h"
#include "ship.h"

typedef struct _game {
// private
    sprite_sheet m_sprites;
    planet m_planets[2];
    ship m_ships[1];
} game;


//
// Constructor
//

game * create_game();


//
// Methods
//

int game_init(game * gm, engine * e);
int game_update(game * gm, engine * e, unsigned int dt);
int game_render(game * gm, graphics * g);
int game_cleanup(game * gm);

#endif
