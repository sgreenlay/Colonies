
#include "graphics.h"

#include "game.h"


// #######################################################################################
// Graphics engine
// #######################################################################################


//
// Constructor
//

graphics * create_graphics()
{
    graphics * g = (graphics *)malloc(sizeof(graphics));
    
    if (g == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate graphics\n");
        return NULL;
    }
    
    return g;
}


//
// Methods
//

int graphics_init(graphics * g, int width, int height)
{
    g->width = width;
    g->height = height;
    
    SDL_SetMainReady();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't initialize SDL [%s]\n", SDL_GetError());
        return 1;
    }
    
    SDL_CreateWindowAndRenderer(
        g->width, g->height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI,
        &g->m_window, &g->m_renderer);
    
    if (g->m_window == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't create window [%s]\n", SDL_GetError());
        return 1;
    }
    
    if (g->m_renderer == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't create g->m_renderer [%s]\n", SDL_GetError());
        return 1;
    }
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(g->m_renderer, g->width, g->height);
    
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    
    g->m_screen = SDL_CreateRGBSurface(
        0, g->width, g->height, 32,
        rmask, gmask, bmask, amask);
    
    if (g->m_screen == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't create screen surface [%s]\n", SDL_GetError());
        return 1;
    }
    
    SDL_Rect screen_bounds = { 0, 0, g->width, g->height };
    
    SDL_FillRect(g->m_screen, &screen_bounds, SDL_MapRGBA(g->m_screen->format, 0x00, 0x00, 0x00, 0xFF));
    
    g->m_texture = SDL_CreateTextureFromSurface(g->m_renderer, g->m_screen);
    
    if (g->m_texture == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't create screen texture [%s]\n", SDL_GetError());
        return 1;
    }
    
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        ENGINE_DEBUG_LOG_ERROR("Failed to intialize SDL_image PNG support [%s]\n", IMG_GetError());
        return 1;
    }
    


    return 0;
}

int graphics_cleanup(graphics * g)
{
    if (g->m_texture)
    {
        SDL_DestroyTexture(g->m_texture);
    }
    
    if (g->m_screen)
    {
        SDL_FreeSurface(g->m_screen);
    }
    
    if (g->m_renderer)
    {
        SDL_DestroyRenderer(g->m_renderer);
    }
    
    if (g->m_window)
    {
        SDL_DestroyWindow(g->m_window);
    }
    
    SDL_Quit();
    
    return 0;
}

int graphics_draw(graphics * g, game * gm)
{
    SDL_SetRenderDrawColor(g->m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->m_renderer);
    
    SDL_Rect source = { 0, 0, g->width, g->height };
    SDL_Rect destination = { 0, 0, g->width, g->height };
    
    SDL_RenderCopy(g->m_renderer, g->m_texture, &source, &destination);
    
    game_render(gm, g);
    
    SDL_RenderPresent(g->m_renderer);
    
    return 0;
}


// #######################################################################################
// Sprite Sheet
// #######################################################################################


//
// Constructor
//

sprite_sheet * create_sprite_sheet()
{
    sprite_sheet * ss = (sprite_sheet *)malloc(sizeof(sprite_sheet));
    
    if (ss == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate sprite sheet\n");
        return NULL;
    }
    
    return ss;
}


//
// Methods
//

int sprite_sheet_init(sprite_sheet * ss, graphics *g, int width, int height, char * path)
{
    ss->width = width;
    ss->height = height;
    
    SDL_Surface * image;
    image = IMG_Load(path);
    
    if (image == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't load image [%s]\n", SDL_GetError());
        return 1;
    }
    
    ss->m_texture = SDL_CreateTextureFromSurface(g->m_renderer, image);
    SDL_FreeSurface(image);
    
    if (ss->m_texture == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't create texture [%s]\n", SDL_GetError());
        return 1;
    }
    
    return 0;
}

int sprite_sheet_cleanup(sprite_sheet * ss)
{
    if (ss->m_texture)
    {
        SDL_DestroyTexture(ss->m_texture);
    }
    
    return 0;
}


// #######################################################################################
// Sprite
// #######################################################################################


//
// Constructor
//

sprite * create_sprite()
{
    sprite * s = (sprite *)malloc(sizeof(sprite));
    
    if (s == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate sprite\n");
        return NULL;
    }
    
    return s;
}


//
// Methods
//

int sprite_init(sprite * s, graphics *g, int width, int height, char * path)
{
    s->width = width;
    s->height = height;
    
    s->m_offset_x = 0;
    s->m_offset_y = 0;
    
    SDL_Surface * image;
    image = IMG_Load(path);
    
    if (image == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't load image [%s]\n", SDL_GetError());
        return 1;
    }
    
    s->m_texture = SDL_CreateTextureFromSurface(g->m_renderer, image);
    SDL_FreeSurface(image);
    
    if (s->m_texture == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("Couldn't create texture [%s]\n", SDL_GetError());
        return 1;
    }
    
    s->m_from_sheet = 0;
    
    return 0;
}

int sprite_init_from_sheet(sprite * s, sprite_sheet *ss, int x, int y, int width, int height)
{
    s->width = width;
    s->height = height;
    
    s->m_offset_x = x;
    s->m_offset_y = y;
    
    s->m_texture = ss->m_texture;
    s->m_from_sheet = 1;
    
    return 0;
}

int sprite_draw(sprite * s, graphics *g, int x, int y)
{
    x = x - s->width / 2;
    y = y - s->height / 2;
    
    SDL_Rect source = { s->m_offset_x, s->m_offset_y, s->width, s->height };
    SDL_Rect destination = { x, y, s->width, s->height };
    
    SDL_RenderCopy(g->m_renderer, s->m_texture, &source, &destination);
    
    return 0;
}

int sprite_draw_scaled(sprite * s, graphics *g, int x, int y, int w, int h)
{
    x = x - w / 2;
    y = y - h / 2;

    SDL_Rect source = { s->m_offset_x, s->m_offset_y, s->width, s->height };
    SDL_Rect destination = { x, y, w, h };
    
    SDL_RenderCopy(g->m_renderer, s->m_texture, &source, &destination);
    
    return 0;
}

int sprite_draw_scaled_and_rotated(sprite * s, graphics *g, int x, int y, int w, int h, float rotation)
{
    SDL_Point center = { w / 2, h / 2 };
    
    x = x - center.x;
    y = y - center.y;

    SDL_Rect source = { s->m_offset_x, s->m_offset_y, s->width, s->height };
    SDL_Rect destination = { x, y, w, h };
    
    SDL_RenderCopyEx(g->m_renderer, s->m_texture, &source, &destination, rotation, &center, SDL_FLIP_NONE);
    
    return 0;
}

int sprite_cleanup(sprite * s)
{
    if (!(s->m_from_sheet) && s->m_texture)
    {
        SDL_DestroyTexture(s->m_texture);
    }
    
    return 0;
}

int font_sheet_init(sprite_sheet * fs, graphics *g, char * path)
{
	SDL_Surface * image;
	image = IMG_Load(path);
	if (image == NULL)
	{
		ENGINE_DEBUG_LOG_ERROR("Couldn't load font sheet [%s]\n", SDL_GetError());
		return 1;
	}
	fs->width = image->w;
	fs->height = image->h;

	fs->m_texture = SDL_CreateTextureFromSurface(g->m_renderer, image);
	SDL_FreeSurface(image);

	if (fs->m_texture == NULL)
	{
		ENGINE_DEBUG_LOG_ERROR("Couldn't create texture [%s]\n", SDL_GetError());
		return 1;
	}

	return 0;
}


//depends on imported font png
const int expected_width = 10;
const int expected_height = 16;
const int height_fix = 1;
const int tiles_per_row = 16;

int drawChar(sprite_sheet* font_sheet, graphics * g, char DrawChar, int x, int y, int size, SDL_Color* color)
{
	float scale = (float)size / expected_width;
	SDL_Rect dstRect = { x, y, expected_width*scale, expected_height*scale };
	int x_cord = (DrawChar% tiles_per_row) * expected_width;
	int y_cord = (DrawChar / tiles_per_row) * (expected_height+height_fix);
	SDL_Rect srcRect = { x_cord, y_cord, expected_width, expected_height + height_fix };
	SDL_SetTextureColorMod(font_sheet->m_texture, color->r, color->g, color->b);
	SDL_RenderCopy(g->m_renderer, font_sheet->m_texture, &srcRect, &dstRect);

	return 0;
}

int drawString(sprite_sheet* font_sheet, graphics * g, char* DrawStr, int x, int y, int size, SDL_Color* color)
{
	int idx;
	int len_s = strlen(DrawStr);
	float scale = (float)size / expected_width;
	for (idx = 0; idx < len_s;idx++)
		drawChar(font_sheet, g, DrawStr[idx], x += expected_width*scale, y, size, color);
	return 0;
}
