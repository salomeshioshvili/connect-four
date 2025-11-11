#include "library.h"

Vec2i direction_to_vector(Direction dir){
    switch (dir)
    {
        case NORTH: return (Vec2i){0, -1};
        case EAST:  return (Vec2i){1, 0};
        case SOUTH: return (Vec2i){0, 1};
        case WEST:  return (Vec2i){-1, 0};
        default:    return (Vec2i){0, 0};
    }
}