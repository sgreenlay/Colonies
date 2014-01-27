
#ifndef _GAME_H_
#define _GAME_H_

#include "engine.h"
#include "graphics.h"
#include "input.h"

#include "planet.h"
#include "ship.h"

typedef enum _hit_test_target_type {
    hit_test_target_none,
    hit_test_target_ship,
    hit_test_target_planet
} hit_test_target_type;

typedef struct _hit_test_target {
    hit_test_target_type type;
    int index;
} hit_test_target;

typedef struct _game {
// private
    sprite_sheet m_sprites;
	sprite_sheet m_font;
    
    int m_cursor_x, m_cursor_y;
    sprite m_cursor_sprite;
    
    planet m_planets[4];
    int m_planet_count;
    
    ship m_ships[1];
    int m_ship_count;

    sprite m_selection_sprite;
    hit_test_target m_current_target;
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
hit_test_target game_hit_test(game * gm, int x, int y);
int game_cleanup(game * gm);

#endif
