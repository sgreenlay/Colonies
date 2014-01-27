
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
    SDL_ShowCursor(0);
    
    return 0;
}

int input_cleanup(input * i)
{
    SDL_ShowCursor(1);
    
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

int input_get_mouse_state(input * i, int * is_left_down, int * is_right_down, int * x, int * y)
{
    Uint32 buttons_state = SDL_GetMouseState(x, y);
	*is_left_down = (0!=(buttons_state & SDL_BUTTON(1))) ;
	*is_right_down = (0 != (buttons_state & SDL_BUTTON(3)));
	    
    return 0;
}

int input_is_key_down(input * i, Key k, char a_z)
{
	//TODO: call this once, save the keystate.
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
		case Key_Space:
			return (keystate[SDL_GetScancodeFromKey(SDLK_SPACE)] != 0);
		case Key_Enter:
			return (keystate[SDL_GetScancodeFromKey(SDLK_RETURN)] != 0);
		case Key_Tab:
			return (keystate[SDL_GetScancodeFromKey(SDLK_TAB)] != 0);
		case Key_Char:
			return (keystate[SDL_GetScancodeFromKey((int)a_z)] != 0);
        default:
            break;
    }
    
    return 0;
}

