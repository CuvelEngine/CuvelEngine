#include "Mesh.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "movement/Camera.hpp"

namespace cuvel
{
	void Mesh::addQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3)
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

	void Mesh::calculateSize()
	{
		for (auto& [pos, color, normal] : this->vertices)
		{
			this->size = glm::max(this->size, pos);
		}
	}

	void Mesh::updateCorners()
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
}
