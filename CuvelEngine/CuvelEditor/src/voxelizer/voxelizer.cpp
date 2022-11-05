#include "voxelizer.hpp"
#include <iostream>
#include <unordered_set>


Voxelizer::Voxelizer(std::string filePath)
{
    readObjFile(filePath, this->faces);

    // Find and set the two coordinates that define the axis aligned bounding box of the model
    this->maxim = glm::vec3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    this->minim = glm::vec3(std::numeric_limits<float>::max(),    std::numeric_limits<float>::max(),    std::numeric_limits<float>::max()   );
    
    for (auto & face : this->faces)
    {
        for (auto & vertex : face.vertices)
        {
            this->maxim = glm::max(this->maxim, vertex);
            this->minim = glm::min(this->minim, vertex);
        } 
    }
}

void Voxelizer::voxelize(int resolution)
{
    std::vector<TriFace> transformedFaces = this->faces;
    this->resize(resolution, transformedFaces);

    std::vector<VoxelPos> voxels;

    for (auto & f : transformedFaces)
    {
        std::vector<VoxelPos> temp = f.voxelize();
        voxels.insert(voxels.end(), temp.begin(), temp.end());
    }

    std::unordered_set<glm::uint32> tmp;

    for (auto & v : voxels)
    {
        if (tmp.find(v.pos) == tmp.end())
        {
            obj(v);
            tmp.insert(v.pos);
        }
    }
}

// Resizes a model so that the largest distance in any of its three axis is the desired resolution (minus a small tolerance, so no face of the
// model touches its bounding box) TODO: ADD BETTER EXPLANATION
void Voxelizer::resize(int resolution, std::vector<TriFace> &faces)
{
    // Determine the longest axis
    float longX = this->maxim.x - this->minim.x;
    float longY = this->maxim.y - this->minim.y;
    float longZ = this->maxim.z - this->minim.z;
    float longestAxis = std::max(longX, std::max(longY, longZ));

    float factor = (resolution - 0.1) / longestAxis;
    glm::vec3 displ = (glm::vec3(resolution, resolution, resolution) - (this->maxim - this->minim) * factor) * 0.5f;

    for (auto & v : faces)
    {
        v.displace(-1.0f * this->minim);  // Move the model so its XXX is (0,0,0)
        v.resize(factor);                 // Resize the model to its desired size
        v.displace(displ);                // Move the model slightly so it is centered inside its bounding box TODO: BETTER EXPLANATION
    }
}
