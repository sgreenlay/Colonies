
#ifndef _SPRITEMAP_H_
#define _SPRITEMAP_H_

typedef struct _sprite_mapping {
    int x, y, w, h;
} sprite_mapping;

typedef enum {
    sprite_map_error,
	sprite_map_cursor,
    sprite_map_selection,
    sprite_map_alien_planet,
    sprite_map_human_planet,
    sprite_map_colony_ship,
    sprite_map_small_fighter1,
	sprite_map_small_fighter2,
	sprite_map_small_fighter3,
	sprite_map_small_fighter4,
    sprite_map_carrier,
    sprite_map_frigate,
    sprite_map_max,
} sprite_map;

static const sprite_mapping map[] = {
    /*  x,   y,   w,   h*/
    {   0,   0,   0,   0 }, /* error */
	{   0, 192,  16,  16 }, /* sprite_map_cursor */
    {  16, 192,  64,  64 }, /* sprite_map_selection */
    { 128,   0, 128, 128 }, /* alien_planet */
    {   0,   0, 128, 128 }, /* human_planet */
    {   0, 128,  64,  64 }, /* colony_ship */
    { 192, 128,  32,  32 }, /* small_fighter1 */
	{ 192, 159,  32,  32 }, /* small_fighter2 */
	{ 224, 128,  32,  32 }, /* small_fighter3 */
	{ 224, 159,  32,  32 }, /* small_fighter4 */
    {  64, 128,  64,  64 }, /* carrier */
    { 128, 128,  64,  64 }, /* frigate */
};

sprite_mapping get_sprite_mapping(sprite_map sm);

#endif
