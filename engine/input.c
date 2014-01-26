
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
    memset(i->m_key_state, 0, Key_Max);
    
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
                case SDLK_UP:
                    i->m_key_state[Key_Up] = 1;
                    break;
                case SDLK_DOWN:
                    i->m_key_state[Key_Down] = 1;
                    break;
                case SDLK_LEFT:
                    i->m_key_state[Key_Left] = 1;
                    break;
                case SDLK_RIGHT:
                    i->m_key_state[Key_Right] = 1;
                    break;
                default:
                    ENGINE_DEBUG_LOG("input: %d\n", event.key.keysym.sym);
                    break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    i->m_key_state[Key_Up] = 0;
                    break;
                case SDLK_DOWN:
                    i->m_key_state[Key_Down] = 0;
                    break;
                case SDLK_LEFT:
                    i->m_key_state[Key_Left] = 0;
                    break;
                case SDLK_RIGHT:
                    i->m_key_state[Key_Right] = 0;
                    break;
                default:
                    ENGINE_DEBUG_LOG("input: %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
    
    return 0;
}

int input_is_key_down(input * i, Key k)
{
    if (k >= Key_Max)
    {
        return 0;
    }
    
    return i->m_key_state[k];
}

