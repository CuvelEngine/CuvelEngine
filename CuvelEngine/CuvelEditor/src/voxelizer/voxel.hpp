#pragma once

#include "glm/glm.hpp"


union VoxelPos
{
    glm::uint32 pos;
    struct {
        glm::uint32 x : 10;
        glm::uint32 y : 10;
        glm::uint32 z : 10;
    };

    VoxelPos(glm::uint32 x, glm::uint32 y, glm::uint32 z) : x(x), y(y), z(z) {};
    VoxelPos(glm::uint32 pos) : pos(pos) {};
};

// Para soportar color en el futuro
/*union Color
{
    glm::uint32 pos;
    struct {
        glm::uint32 x : 10;
        glm::uint32 y : 10;
        glm::uint32 z : 10;
    };
    Color(glm::uint32 x, glm::uint32 y, glm::uint32 z) : x(x), y(y), z(z) {};
    Color(glm::uint32 pos) : pos(pos) {};
};

struct Voxel
{
    VoxelPos pos;
    Color color;
};*/
