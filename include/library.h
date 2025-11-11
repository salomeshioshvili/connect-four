#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x;
    int y;
} Vec2i;

typedef enum {
    NORTH, EAST, SOUTH, WEST
} Direction;

/**
 * @brief Transform a direction to vector.
 * @param dir direction
 */
Vec2i direction_to_vector(Direction dir);

#ifdef __cplusplus
}
#endif