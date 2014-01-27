
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "game_stub.h"
#include "util.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

// #######################################################################################
// Graphics engine
// #######################################################################################

typedef struct _graphics {
    int width, height;

// private
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
    SDL_Surface * m_screen;
    SDL_Texture * m_texture;
} graphics;


//
// Constructor
//

graphics * create_graphics();


//
// Methods
//

int graphics_init(graphics * g, int width, int height);
int graphics_draw(graphics * g, game * gm);
int graphics_cleanup(graphics * g);



// #######################################################################################
// Sprite Sheet
// #######################################################################################

typedef struct _sprite_sheet {
    int width, height;

// private
    SDL_Texture * m_texture;
} sprite_sheet;


//
// Constructor
//

sprite_sheet * create_sprite_sheet();


//
// Methods
//

int sprite_sheet_init(sprite_sheet * ss, graphics *g, int width, int height, char * path);
int sprite_sheet_cleanup(sprite_sheet * ss);


// #######################################################################################
// Sprite
// #######################################################################################

typedef struct _sprite {
    int width, height;

// private
    int m_offset_x, m_offset_y;
    SDL_Texture * m_texture;
    int m_from_sheet;
} sprite;


//
// Constructor
//

sprite * create_sprite();


//
// Methods
//

int sprite_init(sprite * s, graphics *g, int width, int height, char * path);
int sprite_init_from_sheet(sprite * s, sprite_sheet *ss, int x, int y, int width, int height);
int sprite_draw(sprite * s, graphics * g, int x, int y);
int sprite_draw_scaled(sprite * s, graphics *g, int x, int y, int w, int h);
int sprite_draw_scaled_and_rotated(sprite * s, graphics *g, int x, int y, int w, int h, float rotation);
int sprite_cleanup(sprite * s);

// #######################################################################################
// Font Drawing
// #######################################################################################

int font_sheet_init(sprite_sheet * fs, graphics *g, char * path);
int drawChar(sprite_sheet* font_sheet, graphics * g, char DrawChar, int x, int y, int size, SDL_Color color);
int drawString(sprite_sheet* font_sheet, graphics * g, char* DrawStr, int x, int y, int size, SDL_Color color);


#endif
