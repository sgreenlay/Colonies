
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "game_stub.h"
#include "util.h"
#include "graphics.h"
#include "input.h"
#include "clock.h"

typedef struct _engine {
// private
    graphics m_graphics;
    input m_input;
    clock m_clock;
} engine;


//
// Constructor
//

engine * create_engine();


//
// Accessors
//

graphics * engine_get_graphics(engine * e);
input * engine_get_input(engine * e);


//
// Methods
//

int engine_init(engine * e, int width, int height);
void engine_run(engine * e, game * gm);
int engine_cleanup(engine * e);

#endif
