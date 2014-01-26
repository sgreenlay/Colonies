
#ifndef _GAME_H_
#define _GAME_H_

#include "engine.h"
#include "graphics.h"
#include "input.h"

typedef struct _game {
// private
    sprite_sheet m_sprites;
    float m_character_x, m_character_y;
    sprite m_character_sprite;
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
