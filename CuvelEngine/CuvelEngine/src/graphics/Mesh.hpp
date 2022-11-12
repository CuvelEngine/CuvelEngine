#pragma once
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "graphics/Vertex.hpp"

namespace cuvel
{
	class VoxelModel;
	class Camera;

	enum Faces
	{
		UP = 0,
		DOWN = 1,
		NORTH = 2,
		SOUTH = 3,
		WEST = 4,
		EAST = 5,

		fst = UP,
		lst = EAST
	};

	// Base structure containing the buffers of a model.
	// This structure is the library agnostic skeleton of a model
	class Mesh
	{
	public:
		
		virtual void render() = 0;

		void prepareBuffers(uint32_t vertexCount, uint32_t indexCount);
		void loadQuad(glm::u8vec3 coord, glm::u8vec4 color, Faces face);

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		glm::u8vec3 size{};

	};
}
