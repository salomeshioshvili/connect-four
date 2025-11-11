#include "library.h"
#include <gtest/gtest.h>

TEST(DirectionToVector, BasicAssertions) {
    Vec2i vec;

    vec = direction_to_vector(NORTH);
    EXPECT_EQ(vec.x, 0);
    EXPECT_EQ(vec.y, -1);

    vec = direction_to_vector(EAST);
    EXPECT_EQ(vec.x, 1);
    EXPECT_EQ(vec.y, 0);

    vec = direction_to_vector(SOUTH);
    EXPECT_EQ(vec.x, 0);
    EXPECT_EQ(vec.y, 1);

    vec = direction_to_vector(WEST);
    EXPECT_EQ(vec.x, -1);
    EXPECT_EQ(vec.y, 0);
}