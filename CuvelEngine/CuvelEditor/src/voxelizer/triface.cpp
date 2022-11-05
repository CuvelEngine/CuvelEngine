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

void obj(VoxelPos pos)
{
    glm::vec3 v1, v2, v3, v4, v5, v6, v7, v8;

    glm::vec3 v = glm::vec3(pos.x, pos.y, pos.z);

    v1 = v + glm::vec3(-0.5,0.5,-0.5);
    v2 = v + glm::vec3(-0.5,0.5,0.5);
    v3 = v + glm::vec3(0.5,0.5,0.5);
    v4 = v + glm::vec3(0.5,0.5,-0.5);
    v5 = v + glm::vec3(-0.5,-0.5,-0.5);
    v6 = v + glm::vec3(-0.5,-0.5,0.5);
    v7 = v + glm::vec3(0.5,-0.5,0.5);
    v8 = v + glm::vec3(0.5,-0.5,-0.5);

    std::cout << "v " << v1.x << " " << v1.y << " " << v1.z << '\n';
    std::cout << "v " << v2.x << " " << v2.y << " " << v2.z << '\n';
    std::cout << "v " << v3.x << " " << v3.y << " " << v3.z << '\n';
    std::cout << "v " << v4.x << " " << v4.y << " " << v4.z << '\n';
    std::cout << "v " << v5.x << " " << v5.y << " " << v5.z << '\n';
    std::cout << "v " << v6.x << " " << v6.y << " " << v6.z << '\n';
    std::cout << "v " << v7.x << " " << v7.y << " " << v7.z << '\n';
    std::cout << "v " << v8.x << " " << v8.y << " " << v8.z << '\n';

    std::cout << "f " << counter + 1 << " " << counter + 2 << " " << counter + 3 << " " << counter + 4 << " " << '\n';
    std::cout << "f " << counter + 5 << " " << counter + 6 << " " << counter + 7 << " " << counter + 8 << " " << '\n';
    std::cout << "f " << counter + 1 << " " << counter + 4 << " " << counter + 8 << " " << counter + 5 << " " << '\n';
    std::cout << "f " << counter + 1 << " " << counter + 2 << " " << counter + 6 << " " << counter + 5 << " " << '\n';
    std::cout << "f " << counter + 2 << " " << counter + 3 << " " << counter + 7 << " " << counter + 6 << " " << '\n';
    std::cout << "f " << counter + 4 << " " << counter + 3 << " " << counter + 7 << " " << counter + 8 << " " << '\n';

    counter += 8;
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
