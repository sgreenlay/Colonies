
#include "sprite_mapping.h"

sprite_mapping get_sprite_mapping(sprite_map sm)
{
    if (sm < sprite_map_max)
    {
        return map[sm];
    }
    return map[sprite_map_error];
}
