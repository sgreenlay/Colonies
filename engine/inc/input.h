
#ifndef _INPUT_H_
#define _INPUT_H_

#include "util.h"
#include "graphics.h"

typedef enum {
    Key_Up = 0,
    Key_Down,
    Key_Left,
    Key_Right,
    Key_Max
} Key;

typedef struct _input {
// private
    int m_reserved;
} input;


//
// Constructor
//

input * create_input();


//
// Methods
//

int input_init(input * i);
int input_poll(input * i);
int input_get_mouse_state(input * i, int * is_left_down, int * x, int * y);
int input_is_key_down(input * i, Key k);
int input_cleanup(input * i);

#endif