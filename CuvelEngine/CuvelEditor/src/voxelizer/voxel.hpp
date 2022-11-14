#pragma once

#include "glm/glm.hpp"


union VoxelPos
{
    glm::uint32 pos;
    struct {
        glm::uint32 x : 10;
        glm::uint32 y : 10;
        glm::uint32 z : 10;
        glm::uint32 unused : 2;
    };

    VoxelPos(glm::uint32 x, glm::uint32 y, glm::uint32 z) : x(x), y(y), z(z), unused(0) {}
    VoxelPos(glm::uint32 pos) : pos(pos) {};
};
