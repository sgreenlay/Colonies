
#include "util.h"
#include "engine.h"
#include "game.h"

int main(
    int argc,
    char* args[])
{
    engine e;
    game gm;
    
    if (engine_init(&e, 800, 480))
    {
        return 1;
    }
    
    if (game_init(&gm, &e))
    {
        return 1;
    }
    
    engine_run(&e, &gm);
    
    if (game_cleanup(&gm))
    {
        return 1;
    }
    
    if (engine_cleanup(&e))
    {
        return 1;
    }
    
    
    return 0;
}
