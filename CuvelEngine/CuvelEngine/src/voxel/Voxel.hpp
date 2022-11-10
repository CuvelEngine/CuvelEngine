#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


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
		void populateMesh(cuvel::Mesh* mesh);

		glm::u8vec3 size{};
		glm::i16vec3 iSize;
		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;
		std::unordered_map<glm::u8vec3, Voxel> voxels{};

	private:
		bool opaqueNeighbor(glm::i16vec3& coords);

	};
}

