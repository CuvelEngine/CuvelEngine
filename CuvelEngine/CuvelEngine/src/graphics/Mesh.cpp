#include "Mesh.hpp"

#include "movement/Camera.hpp"
#include "voxel/Voxel.hpp"

inline glm::u8vec3 XYZRel[8] = {
	glm::u8vec3(0, 0, 0),
	glm::u8vec3(1, 0, 0),
	glm::u8vec3(0, 1, 0),
	glm::u8vec3(1, 1, 0),
	glm::u8vec3(0, 0, 1),
	glm::u8vec3(1, 0, 1),
	glm::u8vec3(0, 1, 1),
	glm::u8vec3(1, 1, 1),
};

inline uint8_t offIndices[6][6] = {
	{5, 4, 0, 0, 1, 5},
	{3, 2, 6, 6, 7, 3},
	{1, 0, 2, 2, 3, 1}, //{1, 0, 2, 3} -> {0, 1, 2, 2, 3, 0}
	{4, 5, 7, 7, 6, 4}, //{4, 5, 7, 6} -> {0, 1, 2, 2, 3, 0}
	{0, 4, 6, 6, 2, 0},
	{5, 1, 3, 3, 7, 5}
};

inline uint8_t offIndices2[6][4] = {
	{5, 4, 0, 1},
	{3, 2, 6, 7},
	{1, 0, 2, 3},
	{4, 5, 7, 6},
	{0, 4, 6, 2},
	{5, 1, 3, 7}
};


namespace cuvel
{
	void Mesh::prepareBuffers(uint32_t vertexCount, uint32_t indexCount)
	{
		this->vertices.reserve(vertexCount);
		this->indices.reserve(indexCount);
	}

	void Mesh::loadQuad(
		const glm::u8vec3 coord,
		const glm::u8vec4 color,
		Faces face)
	{
		{
			Vertex vertex{};

			vertex.color = color;
			vertex.normal = face;

			//Insert vertices
			vertex.position = coord + XYZRel[offIndices2[face][0]];
			uint32_t v0 = this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices2[face][1]];
			uint32_t v1 = this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices2[face][2]];
			uint32_t v2 = this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices2[face][3]];
			uint32_t v3 = this->insertVertex(vertex);

			// Triangle 1
			this->insertIndex(v0);
			this->insertIndex(v1);
			this->insertIndex(v2);
			// Triangle 2
			this->insertIndex(v2);
			this->insertIndex(v3);
			this->insertIndex(v0);
		}
	}

	uint32_t Mesh::insertVertex(const Vertex vertex)
	{
		uint32_t vtID = static_cast<uint32_t>(this->vertices.size());
		this->vertices.push_back(vertex);
		return vtID;
	}

	void Mesh::insertIndex(uint32_t index)
	{
		this->indices.push_back(index);
	}
}
