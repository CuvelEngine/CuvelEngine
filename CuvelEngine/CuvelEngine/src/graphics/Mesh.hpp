#pragma once
#include <vector>

#include <glm/gtx/hash.hpp>

namespace cuvel
{
	class Camera;

	// The basic structure containing the data for each vertex
	struct Vertex
	{
		glm::u8vec3 position;
		glm::u8vec4 color;
		uint8_t normal;
	};

	// Base structure containing the buffers of a model.
	// This structure is the library agnostic skeleton of a model
	struct Mesh
	{
		void addQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3);

		//TODO: Make sure this doesn't flip in Vulkan (Vulkan has inverted y axis)
		glm::mat4 getModelMatrix();
		glm::mat3 getNormalMatrix();

		void updateMatrices();

		void calculateSize();
		void updateCorners();

		void getRenderStats(uint32_t* vertices, uint32_t* indices);
		void translate(glm::vec3 newPos);

		bool isInsideClippingPlane(Camera* cam);

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		glm::vec3 position{};
		glm::u8vec3 size{};
		glm::vec3 corners[8] = {};

	private:
		glm::mat4 modelMatrix{};
		glm::mat3 normalMatrix{};
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

//right -> 0
//left  -> 1
//back  -> 2
//front -> 3
//up    -> 4
//down  -> 5

#define WHITE_VERT(v, n) {coordList.at(v - 1), glm::u8vec4(255), n}

inline cuvel::Mesh makeCube()
{
	cuvel::Mesh mesh;
	std::vector<glm::u8vec3> coordList;
	coordList.emplace_back(0, 0, 0); //a
	coordList.emplace_back(0, 1, 0); //b
	coordList.emplace_back(1, 1, 0); //c
	coordList.emplace_back(1, 0, 0); //d
	coordList.emplace_back(0, 0, 1); //e
	coordList.emplace_back(0, 1, 1); //f
	coordList.emplace_back(1, 1, 1); //g
	coordList.emplace_back(1, 0, 1); //h

	mesh.addQuad(
		WHITE_VERT(3, 0),
		WHITE_VERT(7, 0),
		WHITE_VERT(8, 0),
		WHITE_VERT(4, 0)
	);

	mesh.addQuad(
		WHITE_VERT(1, 1),
		WHITE_VERT(5, 1),
		WHITE_VERT(6, 1),
		WHITE_VERT(2, 1)
	);

	mesh.addQuad(
		WHITE_VERT(7, 2),
		WHITE_VERT(3, 2),
		WHITE_VERT(2, 2),
		WHITE_VERT(6, 2)
	);

	mesh.addQuad(
		WHITE_VERT(4, 3),
		WHITE_VERT(8, 3),
		WHITE_VERT(5, 3),
		WHITE_VERT(1, 3)
	);

	mesh.addQuad(
		WHITE_VERT(8, 4),
		WHITE_VERT(7, 4),
		WHITE_VERT(6, 4),
		WHITE_VERT(5, 4)
	);

	mesh.addQuad(
		WHITE_VERT(3, 5),
		WHITE_VERT(4, 5),
		WHITE_VERT(1, 5),
		WHITE_VERT(2, 5)
	);

	return mesh;
}