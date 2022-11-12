#pragma once
#include "VoxelReader.hpp"

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <tsl/robin_map.h>


namespace cuvel
{
	class Mesh;

	class Voxel
	{
	public:
		 glm::u8vec3 pos;
		 glm::u8vec4 color;

		Voxel(glm::u8vec3 pos, glm::u8vec4 color) : pos(pos), color(color) {};

	};

	class VoxelModel
	{
	public:
		VoxelModel();
		explicit VoxelModel(std::string filepath);
		void populateMesh(std::shared_ptr<cuvel::Mesh> mesh);

		glm::u8vec3 size{};
		glm::i16vec3 iSize;
		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;
		tsl::robin_map<VOXEL_HASH> voxels{};

	private:
		bool opaqueNeighbor(glm::i16vec3& coords);
	};
}

