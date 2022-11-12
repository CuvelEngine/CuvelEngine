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

cuvel::Vertex tempVertex{};
uint32_t tempIndex = 0;

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

		tempVertex.color = color;
		tempVertex.normal = face;

		//Insert vertices
		tempVertex.position = coord + XYZRel[offIndices2[face][0]];
		this->vertices.push_back(tempVertex);
		tempVertex.position = coord + XYZRel[offIndices2[face][1]];
		this->vertices.push_back(tempVertex);
		tempVertex.position = coord + XYZRel[offIndices2[face][2]];
		this->vertices.push_back(tempVertex);
		tempVertex.position = coord + XYZRel[offIndices2[face][3]];
		this->vertices.push_back(tempVertex);

		// Triangle 1
		this->indices.push_back(tempIndex);
		this->indices.push_back(tempIndex + 1);
		this->indices.push_back(tempIndex + 2);
		// Triangle 2
		this->indices.push_back(tempIndex + 2);
		this->indices.push_back(tempIndex + 3);
		this->indices.push_back(tempIndex);
		tempIndex += 4;
		
	}
}
