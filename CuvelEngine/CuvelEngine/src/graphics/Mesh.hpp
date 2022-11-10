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
		//TODO: Make sure this doesn't flip in Vulkan (Vulkan has inverted y axis)
		glm::mat4 getModelMatrix();
		glm::mat3 getNormalMatrix();
		void updateMatrices();

		void getRenderStats(uint32_t* vertices, uint32_t* indices);
		void translate(glm::vec3 newPos);

		void updateCorners();
		bool isInsideClippingPlane(Camera* cam);

		void loadQuad(glm::u8vec3 coord, glm::u8vec4 color, Faces face);

		void insertVertex(Vertex vertex);

		std::vector<Vertex> vertices;
		std::unordered_map<Vertex, uint32_t> uniqueVertices;
		std::vector<uint32_t> indices;
		glm::vec3 position{};
		glm::u8vec3 size{};
		glm::vec3 corners[8] = {};

	private:
		glm::mat4 modelMatrix{};
		glm::mat3 normalMatrix{};
	};
}
