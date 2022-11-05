#pragma once
#include "settings/GraphicOptions.hpp"

#include <vector>

namespace cuvel
{
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
		void addQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3)
		{
			uint32_t index = this->vertices.size();

			this->vertices.push_back(v0);
			this->vertices.push_back(v1);
			this->vertices.push_back(v2);
			this->vertices.push_back(v3);

			this->indices.push_back(index);
			this->indices.push_back(index + 1);
			this->indices.push_back(index + 2);
			this->indices.push_back(index);
			this->indices.push_back(index + 2);
			this->indices.push_back(index + 3);
		}
		
		//TODO: Make sure this doesn't flip in Vulkan (Vulkan has inverted y axis)
		glm::mat4 getModelMatrix()
		{
			return this->modelMatrix;
		}

		glm::mat3 getNormalMatrix()
		{
			return this->normalMatrix;
		}

		void updateMatrices()
		{
			// creates a model matrix with the model position, simple as it looks
			this->modelMatrix = glm::translate(glm::mat4(1.f), this->position);
			// creates a normal matrix with the model matrix
			this->normalMatrix = glm::transpose(glm::inverse(glm::mat3(this->modelMatrix)));
			this->updateCorners();
		}

		void calculateSize()
		{
			uint8_t mx = 0;
			uint8_t my = 0;
			uint8_t mz = 0;
			for (auto& [pos, color, normal] : this->vertices)
			{
				mx = std::max(pos.x, mx);
				my = std::max(pos.y, my);
				mz = std::max(pos.z, mz);
			}
			this->size = glm::u8vec3(mx, my, mz);
		}

		void updateCorners()
		{
			if (this->size == glm::u8vec3(0)) this->calculateSize();
			this->corners[0] = this->position;
			this->corners[1] = glm::vec3(this->position.x + this->size.x, this->position.y, this->position.z);
			this->corners[2] = glm::vec3(this->position.x, this->position.y + this->size.y, this->position.z);
			this->corners[3] = glm::vec3(this->position.x, this->position.y, this->position.z + this->size.z);
			this->corners[4] = glm::vec3(this->position.x + this->size.x, this->position.y + this->size.y, this->position.z);
			this->corners[5] = glm::vec3(this->position.x + this->size.x, this->position.y, this->position.z + this->size.z);
			this->corners[6] = glm::vec3(this->position.x, this->position.y + this->size.y, this->position.z + this->size.z);
			this->corners[6] = glm::vec3(this->position.x + this->size.x, this->position.y + this->size.y, this->position.z + this->size.z);
		}

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