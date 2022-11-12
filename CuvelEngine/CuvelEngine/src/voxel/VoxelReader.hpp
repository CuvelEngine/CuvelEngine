#pragma once

#include <string>
#include <glm/fwd.hpp>

#include "utils/FlagVector.hpp"

namespace cuvel
{
	class Voxel;

	void readVoxmFile(
		std::string& filePath,
		std::vector<Voxel>* voxels,
		cuvel::FlagVector* neighborFlags,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices);

	void generateVoxmSample(
		std::vector<Voxel>* voxels,
		cuvel::FlagVector* neighborFlags,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices);
}
