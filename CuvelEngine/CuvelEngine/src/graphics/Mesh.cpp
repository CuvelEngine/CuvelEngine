#include "Mesh.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "movement/Camera.hpp"
#include "movement/Camera.hpp"
#include "movement/Camera.hpp"
#include "movement/Camera.hpp"
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
	{1, 0, 2, 2, 3, 1},
	{4, 5, 7, 7, 6, 4},
	{0, 4, 6, 6, 2, 0},
	{5, 1, 3, 3, 7, 5}
};


namespace cuvel
{
	glm::mat4 Mesh::getModelMatrix()
	{
		return this->modelMatrix;
	}

	glm::mat3 Mesh::getNormalMatrix()
	{
		return this->normalMatrix;
	}

	void Mesh::updateMatrices()
	{
		// creates a model matrix with the model position, simple as it looks
		this->modelMatrix = glm::translate(glm::mat4(1.f), this->position);
		// creates a normal matrix with the model matrix
		this->normalMatrix = glm::transpose(glm::inverse(glm::mat3(this->modelMatrix)));
		this->updateCorners();
	}

	void Mesh::updateCorners()
	{
		this->corners[0] = this->position;
		this->corners[1] = glm::vec3(this->position.x + this->size.x, this->position.y, this->position.z);
		this->corners[2] = glm::vec3(this->position.x, this->position.y + this->size.y, this->position.z);
		this->corners[3] = glm::vec3(this->position.x, this->position.y, this->position.z + this->size.z);
		this->corners[4] = glm::vec3(this->position.x + this->size.x, this->position.y + this->size.y, this->position.z);
		this->corners[5] = glm::vec3(this->position.x + this->size.x, this->position.y, this->position.z + this->size.z);
		this->corners[6] = glm::vec3(this->position.x, this->position.y + this->size.y, this->position.z + this->size.z);
		this->corners[6] = glm::vec3(this->position.x + this->size.x, this->position.y + this->size.y, this->position.z + this->size.z);
	}

	void Mesh::getRenderStats(uint32_t* vertices, uint32_t* indices)
	{
		*vertices += this->vertices.size();
		*indices += this->indices.size();
	}
	void Mesh::translate(glm::vec3 newPos)
	{
		if (this->position == newPos) return;
		this->position = newPos;
		this->updateMatrices();
	}

	bool Mesh::isInsideClippingPlane(Camera* cam)
	{
		// Every corner has to be outside of the clipping plane for the model to be culled
		for (glm::vec3& corner : this->corners)
		{
			if (cam->pointInFrustum(corner) != FrustResult::OUTSIDE)
			{
				return true;
			}
		}
		return false;
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

			//First triangle
			vertex.position = coord + XYZRel[offIndices[face][0]];
			this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices[face][1]];
			this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices[face][2]];
			this->insertVertex(vertex);

			//Second triangle
			vertex.position = coord + XYZRel[offIndices[face][3]];
			this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices[face][4]];
			this->insertVertex(vertex);
			vertex.position = coord + XYZRel[offIndices[face][5]];
			this->insertVertex(vertex);
		}
	}

	void Mesh::insertVertex(const Vertex vertex)
	{
		uint32_t vtID;
		if (!uniqueVertices.contains(vertex)) {
			vtID = static_cast<uint32_t>(this->vertices.size());
			uniqueVertices[vertex] = vtID;
			this->vertices.push_back(vertex);
		}
		else
		{
			vtID = uniqueVertices[vertex];
		}
		this->indices.push_back(vtID);
	}
}
