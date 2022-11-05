#pragma once

#include "voxel.hpp"
#include <vector>


struct TriFace
{
    glm::vec3 vertices[3] = {};

    TriFace(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3);
    void resize(const float factor);
    void displace(const glm::vec3 displacement);
    bool intersectWithLine(glm::vec3 ray, glm::vec3 origin, glm::vec3 &intersection) const;
    std::vector<VoxelPos> voxelize();
};
