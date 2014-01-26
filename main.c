
#define SDL_MAIN_HANDLED
#include "SDL.h"

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
    
    SDL_Window * window = SDL_CreateWindow(
        "ggj14",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_ALLOW_HIGHDPI);
    
    if (window == NULL)
    {
        printf("Couldn't create window [%s]\n", SDL_GetError());
        goto exit;
    }
    
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
        
        // TODO: Render game state
    }

exit:
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
