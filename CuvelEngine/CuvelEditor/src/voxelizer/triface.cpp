#include "triface.hpp"
#include <limits>
#include <iostream>


int counter = 0;

TriFace::TriFace(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3)
{
    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;
}

void TriFace::resize(const float factor)
{
    this->vertices[0] *= factor;
    this->vertices[1] *= factor;
    this->vertices[2] *= factor;
}

void TriFace::displace(const glm::vec3 displacement)
{
    this->vertices[0] += displacement;
    this->vertices[1] += displacement;
    this->vertices[2] += displacement;
}

// Möller–Trumbore intersection algorithm
bool TriFace::intersectWithLine(glm::vec3 ray, glm::vec3 origin, glm::vec3 &intersection) const
{
    constexpr float epsilon = 0.0000001f;

    glm::vec3 edge1 = this->vertices[1] - this->vertices[0];
    glm::vec3 edge2 = this->vertices[2] - this->vertices[0];
    glm::vec3 h = glm::cross(ray, edge2);
    float a = glm::dot(edge1, h);
    if (a > -epsilon && a < epsilon)
    {
        return false;  // The ray is parallel to the triangle
    }

    float f = 1.0f / a;
    glm::vec3 s = origin - this->vertices[0];
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f)
    {
        return false;
    }

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(ray, q);
    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    // Now we find the intersection
    float t = f * glm::dot(edge2, q);
    intersection = origin + ray * t;

    return true;
}

std::vector<VoxelPos> TriFace::voxelize()
{

    // First we calculate the grid aligned bounding box of the triangle
    glm::vec3 maxim(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    glm::vec3 minim(std::numeric_limits<float>::max(),    std::numeric_limits<float>::max(),    std::numeric_limits<float>::max()   );

    for (auto & vertex : this->vertices)
    {
        maxim = glm::max(maxim, vertex);
        minim = glm::min(minim, vertex);
    }

    maxim = glm::ceil(maxim);
    minim = glm::floor(minim);

    // Now, we test for collsions between the triangle and lines that go through the center of the voxels and are aligned with
    // the axes. If a collision is detected, the voxel(s) that contain the intersection point are added to the list of voxels
    std::vector<VoxelPos> grid;
    glm::vec3 intersec;

    // Aligned with X Axis
    for (float y = minim.y + 0.5; y < maxim.y; ++y)
    {
        for (float z = minim.z + 0.5; z < maxim.z; ++z)
        {
            if (this->intersectWithLine(glm::vec3(1, 0, 0), glm::vec3(minim.x, y, z), intersec))
            {
                glm::ivec3 voxelPos = glm::floor(intersec);
                grid.emplace_back(VoxelPos(voxelPos.x, voxelPos.y, voxelPos.z));
                if (roundf(intersec.x) == intersec.x)
                {
                    voxelPos -= glm::ivec3(1, 0, 0);
                    grid.emplace_back(VoxelPos(voxelPos.x, voxelPos.y, voxelPos.z));
                }
            }
        }
    }

    // Aligned with Y Axis
    for (float x = minim.x + 0.5; x < maxim.x; ++x)
    {
        for (float z = minim.z + 0.5; z < maxim.z; ++z)
        {
            if (this->intersectWithLine(glm::vec3(0, 1, 0), glm::vec3(x, minim.y, z), intersec))
            {
                glm::ivec3 voxelPos = glm::floor(intersec);
                grid.emplace_back(VoxelPos(voxelPos.x, voxelPos.y, voxelPos.z));
                if (roundf(intersec.y) == intersec.y)
                {
                    voxelPos -= glm::ivec3(0, 1, 0);
                    grid.emplace_back(VoxelPos(voxelPos.x, voxelPos.y, voxelPos.z));
                }
            }
        }
    }

    // Aligned with Z Axis
    for (float x = minim.x + 0.5; x < maxim.x; ++x)
    {
        for (float y = minim.y + 0.5; y < maxim.y; ++y)
        {
            if (this->intersectWithLine(glm::vec3(0, 0, 1), glm::vec3(x, y, minim.z), intersec))
            {
                glm::ivec3 voxelPos = glm::floor(intersec);
                grid.emplace_back(VoxelPos(voxelPos.x, voxelPos.y, voxelPos.z));
                if (roundf(intersec.z) == intersec.z)
                {
                    voxelPos -= glm::ivec3(0, 0, 1);
                    grid.emplace_back(VoxelPos(voxelPos.x, voxelPos.y, voxelPos.z));
                }
            }
        }
    }

    return grid;
}
