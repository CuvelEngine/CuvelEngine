#pragma once

#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

namespace cuvel
{
	class Voxel;

	void readVoxmFile(
		std::string& filePath,
		std::unordered_map<glm::u8vec3, Voxel>* voxels,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices);

	void generateVoxmSample(
		std::unordered_map<glm::u8vec3, Voxel>* voxels,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices);
}
