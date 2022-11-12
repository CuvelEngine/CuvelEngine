#pragma once

#include <string>
#include <glm/fwd.hpp>
#include <tsl/robin_map.h>

# define VOXEL_HASH glm::u8vec3, Voxel, std::hash<glm::u8vec3>, std::equal_to<glm::u8vec3>, std::allocator<std::pair<glm::u8vec3, Voxel>>, true

namespace cuvel
{
	class Voxel;

	void readVoxmFile(
		std::string& filePath,
		tsl::robin_map<VOXEL_HASH>* voxels,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices);

	void generateVoxmSample(
		tsl::robin_map<VOXEL_HASH>* voxels,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices);
}
