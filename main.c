
#define SDL_MAIN_HANDLED
#include <SDL.h>

#define FRAMES_PER_SECOND 60
#define MS_PER_FRAME ((int) (1000 / (FRAMES_PER_SECOND)))

#define PIXELS_Y_PER_FRAME  4
#define PIXELS_X_PER_FRAME  4
int main(
    int argc,
    char* args[])
{
    SDL_SetMainReady();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Couldn't initialize SDL [%s]\n", SDL_GetError());
        goto exit;
    }
    
    SDL_Window * window;
    SDL_Renderer * renderer;
    
    SDL_CreateWindowAndRenderer(
        800, 480,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI,
        &window, &renderer);
    
    if (window == NULL)
    {
        printf("Couldn't create window [%s]\n", SDL_GetError());
        goto exit;
    }
    
    if (renderer == NULL)
    {
        printf("Couldn't create renderer [%s]\n", SDL_GetError());
        goto exit;
    }
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 800, 480);
    
    SDL_Surface * screen;
    SDL_Rect screenRect = { 0, 0, 800, 480 };
    
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
    
    screen = SDL_CreateRGBSurface(
        0, 800, 480, 32,
        rmask, gmask, bmask, amask);
    
    if (screen == NULL)
    {
        printf("Couldn't create screen surface [%s]\n", SDL_GetError());
        goto exit;
    }
    
    SDL_FillRect(screen, &screenRect, SDL_MapRGBA(screen->format, 0x00, 0x00, 0xff, 0xFF));
    
    SDL_Texture * screenTexture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_FreeSurface(screen);
    
    // TODO: Initialize game
    
    SDL_Surface * image;
    image = SDL_LoadBMP("assets/test.bmp");
    
    if (image == NULL)
    {
        printf("Couldn't load image [%s]\n", SDL_GetError());
        goto exit;
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    
    if (texture == NULL)
    {
        printf("Couldn't create texture [%s]\n", SDL_GetError());
        goto exit;
    }
    
    int image_x = 400 - 25;
    int image_y = 240 - 25;
    int game_loop = 1;
	int delta_x = 0;
	int delta_y = 0;

    SDL_Event event;
	unsigned int start_loop_time = SDL_GetTicks();

    while (game_loop) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                game_loop = 0;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:
						delta_x = -PIXELS_X_PER_FRAME;
                        break;
                    case SDLK_RIGHT:
						delta_x = PIXELS_X_PER_FRAME;
                        break;
                    case SDLK_UP:
						delta_y = -PIXELS_Y_PER_FRAME;
                        break;
                    case SDLK_DOWN:
						delta_y = PIXELS_Y_PER_FRAME;
                        break;
                    default:
                        printf("KeyDown event: %d\n", event.key.keysym.sym);
                        break;
                }
            }
			else if (event.type == SDL_KEYUP)
				switch (event.key.keysym.sym)
			{
				case SDLK_LEFT:
					delta_x = 0;
					break;
				case SDLK_RIGHT:
					delta_x = 0;
					break;
				case SDLK_UP:
					delta_y = 0;
					break;
				case SDLK_DOWN:
					delta_y = 0;
					break;
				default:
					printf("KeyUp event: %d\n", event.key.keysym.sym);
					break;
			}
        }
        
        // TODO: Update game state -- updating
		{
			image_x += delta_x;
			image_y += delta_y;
			//TODO: change to contants once we know image size
			if (image_x > (800 - 50)) image_x = 800 - 50;
			if (image_y > (480 - 50)) image_y = 480 - 50;
			if (image_x < 0) image_x = 0;
			if (image_y < 0) image_y = 0;
		}
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_Rect screenSource = { 0, 0, 800, 480 };
        SDL_Rect screenDestination = { 0, 0, 800, 480 };
        
        SDL_RenderCopy(renderer, screenTexture, &screenSource, &screenDestination);
        
        // TODO: Render game state
        
        SDL_Rect source = { 0, 0, 50, 50 };
        SDL_Rect destination = { image_x, image_y, 50, 50 };
        SDL_RenderCopy(renderer, texture, &source, &destination);
        
        SDL_RenderPresent(renderer);
		unsigned int end_loop_time = SDL_GetTicks();
		long sleep_time = MS_PER_FRAME - ((long)end_loop_time - start_loop_time);
		if (sleep_time > 0)
			SDL_Delay((unsigned int)sleep_time);
		//printf("%i\n", (unsigned int)sleep_time);
		start_loop_time = end_loop_time;
		
    }
    

exit:
    // TODO: Destroy game
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
