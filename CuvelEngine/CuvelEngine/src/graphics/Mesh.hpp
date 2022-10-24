#pragma once
#include "settings/GraphicOptions.hpp"

#include <vector>

namespace cuvel
{
	// The basic structure containing the data for each vertex
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};

	// Base structure containing the buffers of a model.
	// This structure is the library agnostic skeleton of a model
	struct Mesh
	{
		//TODO: Make sure this doesn't flip in Vulkan (Vulkan has inverted y axis)
		glm::mat4 getModelMatrix()
		{
			// creates a model matrix with the model position, simple as it looks
			return glm::translate(glm::mat4(1.f), this->position);
		}

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		glm::vec3 position;
	};
}



// Create a hash function for Vertex struct.
// This is simply so we can use it as key for unordered_map
// Source: https://www.youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

namespace std {
	template <>
	struct hash<cuvel::Vertex> {
		size_t operator()(cuvel::Vertex const& vertex) const {
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color);
			return seed;
		}
	};
}
