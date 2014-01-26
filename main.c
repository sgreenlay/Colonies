
#define SDL_MAIN_HANDLED
#include <SDL.h>

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
    
    SDL_Event event;
    
    while (game_loop) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                game_loop = 0;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                printf("Key event: %d\n", event.key.keysym.sym);
                break;
            }
        }
        
        // TODO: Update game state
        
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
    }
    

exit:
    // TODO: Destroy game
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
