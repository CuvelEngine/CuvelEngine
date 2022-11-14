#pragma once

#include "modelreader.hpp"
#include <unordered_set>


class Voxelizer
{
    std::vector<TriFace> faces;
    glm::vec3 maxim;
    glm::vec3 minim;

public:
    Voxelizer(std::string filePath);
    void voxelize(int resolution);

private:
    void resize(int resolution, std::vector<TriFace> &faces);
    void writeVoxmFile(std::string filePath, int res, std::unordered_set<glm::uint32>& voxels);
};
