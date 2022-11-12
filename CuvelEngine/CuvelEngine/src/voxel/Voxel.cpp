#include "Voxel.hpp"

#include "graphics/Mesh.hpp"

inline glm::i8vec3 neighOffsets[] = {
	glm::i8vec3( 0, -1,  0),
	glm::i8vec3( 0,  1,  0),
	glm::i8vec3( 0,  0, -1),
	glm::i8vec3( 0,  0,  1),
	glm::i8vec3(-1,  0,  0),
	glm::i8vec3( 1,  0,  0)
};

namespace cuvel
{
	VoxelModel::VoxelModel()
	{
		generateVoxmSample(&voxels, &size, &vertexCount, &indexCount);
		this->iSize = glm::i16vec3(size);
	}

	VoxelModel::VoxelModel(std::string filepath)
	{
		readVoxmFile(filepath, &voxels, &size, &vertexCount, &indexCount);
		this->iSize = glm::i16vec3(size);
	}

	void VoxelModel::populateMesh(std::shared_ptr<cuvel::Mesh> mesh)
	{
		//mesh->prepareBuffers(this->vertexCount, this->indexCount);
		//TODO: Give proper vertex count in the voxm file
		mesh->prepareBuffers(1322000, 1983000);
		for (auto& [pos, voxel] : this->voxels)
		{
			if (voxel.color.a == 0) continue;
			for (int f = Faces::fst; f < Faces::lst + 1; f++)
			{
				const auto face = static_cast<Faces>(f);
				glm::i16vec3 neighPos = glm::i16vec3(voxel.pos) + glm::i16vec3(neighOffsets[f]);
				if (!opaqueNeighbor(neighPos))
				{
					mesh->loadQuad(voxel.pos, voxel.color, face);
				}
			}
		}
		mesh->size = this->size;

	}

	bool VoxelModel::opaqueNeighbor(glm::i16vec3& coords)
	{
		if (coords.x < 0 || coords.y < 0 || coords.z < 0) return false;
		if (coords.x >= iSize.x || coords.y >= iSize.y || coords.z >= iSize.z) return false;

		return this->voxels.contains(glm::u8vec3(coords)) && this->voxels.at(coords).color.a == 255;
	}
}

