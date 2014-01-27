
#ifndef _GAME_H_
#define _GAME_H_

#include "engine.h"
#include "graphics.h"
#include "input.h"

#include "planet.h"
#include "ship.h"

typedef enum _SelectedObject{
	SONothing =0,
	SOPlanet = 1,
	SOShip = 2
} SelectedObject;


typedef struct _game {
// private
    sprite_sheet m_sprites;
	sprite_sheet m_font;
    
    int m_cursor_x, m_cursor_y;
    sprite m_cursor_sprite;
    
    planet m_planets[2];
    ship m_ships[1];

	int selected_active; //if something is selected or not
	SelectedObject selected_type;   //nothing, planet or ship
	int selection_id;    //id into the array of the object

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
