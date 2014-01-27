
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

// TODO: make this shared by planet + ship
typedef enum _hit_test_control_type {
    hit_test_control_none,
    hit_test_control_human,
    hit_test_control_alien
} hit_test_control_type;

typedef struct _hit_test_target {
    hit_test_target_type type;
    hit_test_control_type controller;
    int index;
} hit_test_target;

typedef enum _menu_status {
	MENU_CREDITS, //transition to main
	MENU_MAIN,    //either creds, game, or quit
	MENU_GAME,    //transition to pause, win, lose
	MENU_PAUSE,   //game, quit
	MENU_WIN,     //main
	MENU_LOSE,    //main
	MENU_QUIT     //nobody returns
} menu_status;

typedef struct _game {
// private
    sprite_sheet m_sprites;
	sprite_sheet m_font;
    
    int m_cursor_x, m_cursor_y;
    sprite m_cursor_sprite;
    
    planet m_planets[4];
    int m_planet_count;
    
    ship m_ships[2];
    int m_ship_count;

    sprite m_selection_sprite;
    hit_test_target m_current_target;
	menu_status game_status;
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
int game_render_ships(game *gm, graphics* g);
int game_render_planets(game *gm, graphics* g);

int game_update_ships(game * gm, engine * e, int is_mouse_left_down, int is_mouse_right_down, unsigned int dt);
hit_test_target game_hit_test(game * gm, int x, int y);
int game_cleanup(game * gm);

#endif
