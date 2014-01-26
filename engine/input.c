
#include "input.h"


//
// Constructor
//

input * create_input()
{
    input * i = (input *)malloc(sizeof(input));
    
    if (i == NULL)
    {
        ENGINE_DEBUG_LOG_ERROR("ERROR: Failed to allocate input\n");
        return NULL;
    }
    
    return i;
}


//
// Methods
//

int input_init(input * i)
{
    // Do nothing
    
    return 0;
}

int input_cleanup(input * i)
{
    // Do nothing
    
    return 0;
}

int input_poll(input * i)
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            return 1;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    return 1;
                default:
                    break;
            }
        }
    }
    
    return 0;
}

int input_get_mouse_position(input * i, int * x, int * y)
{
    SDL_GetRelativeMouseState(x, y);
    
    return 0;
}

int input_is_key_down(input * i, Key k)
{
    const Uint8 * keystate = SDL_GetKeyboardState(NULL);
    
    switch (k)
    {
        case Key_Up:
            return (keystate[SDL_GetScancodeFromKey(SDLK_UP)] != 0);
        case Key_Down:
            return (keystate[SDL_GetScancodeFromKey(SDLK_DOWN)] != 0);
        case Key_Left:
            return (keystate[SDL_GetScancodeFromKey(SDLK_LEFT)] != 0);
        case Key_Right:
            return (keystate[SDL_GetScancodeFromKey(SDLK_RIGHT)] != 0);
        default:
            break;
    }
    
    return 0;
}
