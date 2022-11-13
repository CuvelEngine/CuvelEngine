#include "voxelizer.hpp"
#include <iostream>
#include <fstream>


void writeVoxmFile(std::string filePath, std::unordered_set<glm::uint32>& voxels);

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
        if (!tmp.contains(v.pos))
        {
            tmp.insert(v.pos);
        }
    }

    writeVoxmFile("caquita.voxm", resolution, tmp);
}

// Resizes a model so that the largest distance in any of its three axis is the desired resolution (minus a small tolerance, so no face of the
// model touches its bounding box) TODO: ADD BETTER EXPLANATION
void Voxelizer::resize(int resolution, std::vector<TriFace> &faces)
{
    // Determine the longest axis
    glm::vec3 axisLengths = this->maxim - this->minim;
    float longestAxis = std::max(axisLengths.x, std::max(axisLengths.y, axisLengths.z));

    float factor = (resolution - 0.1) / longestAxis;
    glm::vec3 displ = (glm::ceil(axisLengths * factor) - ((this->maxim - this->minim) * factor)) * 0.5f;

    for (auto & v : faces)
    {
        v.displace(-1.0f * this->minim);  // Move the model so its XXX is (0,0,0)
        v.resize(factor);                 // Resize the model to its desired size
        v.displace(displ);                // Move the model slightly so it is centered inside its bounding box TODO: BETTER EXPLANATION
    }
}

void printSet(std::unordered_set<glm::uint32> const& s)
{
    for (auto const& i : s) {
        std::cout << i << " ";
    }
}

void Voxelizer::writeVoxmFile(std::string filePath, int res, std::unordered_set<glm::uint32>& voxels)
{
    std::ofstream wf(filePath, std::ios::out | std::ios::binary);
    if (!wf) {
        std::cout << "Cannot open file" << "\n";
        return;
    }

    glm::vec3 axisLengths = this->maxim - this->minim;
    float longestAxis = std::max(axisLengths.x, std::max(axisLengths.y, axisLengths.z));
    float factor = (res - 0.1) / longestAxis;
    glm::vec3 modelSizeTmp = axisLengths * factor;
    glm::u8vec3 modelSize = glm::ceil(modelSizeTmp);

    wf.write(reinterpret_cast<char*>(&modelSize.x), sizeof(modelSize.x));
    wf.write(reinterpret_cast<char*>(&modelSize.y), sizeof(modelSize.y));
    wf.write(reinterpret_cast<char*>(&modelSize.z), sizeof(modelSize.z));

    glm::uint32 vertexCount = 0x63697266, indexCount = 0x646F6D68;
    wf.write(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));
    wf.write(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));

    glm::uint8 zeroFlagAir = 0;
    wf.write(reinterpret_cast<char*>(&zeroFlagAir), sizeof(zeroFlagAir));


	glm::uint32 white(0xFFFFFFFF);
    glm::uint8 opaque = 0xFF;
    bool lastAir = true;
    for (glm::uint8 z = 0; z < modelSize.z; ++z)
    {
	    for (glm::uint8 y = 0; y < modelSize.y; ++y)
	    {
		    for (glm::uint8 x = 0; x < modelSize.x; ++x)
		    {
                VoxelPos pos(x, y, z);
			    if (voxels.contains(pos.pos)) // Not Air
			    {
                    if (lastAir)
                    {

                        // Write the coordinates of the ...
                        wf.write(reinterpret_cast<char*>(&x), sizeof(x));
                        wf.write(reinterpret_cast<char*>(&y), sizeof(y));
                        wf.write(reinterpret_cast<char*>(&z), sizeof(z));
                    }
                    
                    // Write the color (ARGB) of the voxel
                    //wf.write(reinterpret_cast<char*>(&white), sizeof(white));
                    glm::uint8 r = x, g = y, b = z;

                    wf.write(reinterpret_cast<char*>(&opaque), sizeof(opaque));
                    wf.write(reinterpret_cast<char*>(&x), sizeof(x));
                    wf.write(reinterpret_cast<char*>(&y), sizeof(y));
                    wf.write(reinterpret_cast<char*>(&z), sizeof(z));

                    lastAir = false;
			    }
                else if (lastAir == false) // Air block && last block NOT air
                {
                    wf.write(reinterpret_cast<char*>(&zeroFlagAir), sizeof(zeroFlagAir));
                    lastAir = true;
                }

		    }
	    }
    }

    wf.close();
}
