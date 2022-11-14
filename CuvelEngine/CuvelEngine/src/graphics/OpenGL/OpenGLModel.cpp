#include "OpenGLModel.hpp"

#include <glm/gtx/vector_angle.hpp>

namespace cuvel
{
	OpenGLModel::OpenGLModel(const std::shared_ptr<Mesh>& mesh, uint32_t coreProgram, bool hasLighting)
	{
		// Simply move the given mesh into the model
		this->mesh = std::dynamic_pointer_cast<OpenGLMesh> (mesh);
		this->coreProgram = coreProgram;
		this->hasLighting = hasLighting ? 1 : 0;
		this->updateMatrices();
	}

	void OpenGLModel::loadUniform()
	{
		// Update the model matrix uniform for this model
		glUniformMatrix4fv(glGetUniformLocation(this->coreProgram, "ModelMatrix"), 1, false, glm::value_ptr(this->getModelMatrix()));

		// Update the normal matrix as well
		glUniformMatrix3fv(glGetUniformLocation(this->coreProgram, "NormalMatrix"), 1, false, glm::value_ptr(this->getNormalMatrix()));

		// Update the lighting flag
		glUniform1ui(glGetUniformLocation(this->coreProgram, "hasLighting"), this->hasLighting);

	}

	void OpenGLModel::render()
	{
		this->loadUniform();
		this->mesh->render();
	}

	void OpenGLModel::getRenderStats(uint32_t* vertices, uint32_t* indices)
	{
		*vertices += this->mesh->vertices.size();
		*indices += this->mesh->indices.size();
	}

	glm::mat4 OpenGLModel::getModelMatrix()
	{
		return this->modelMatrix;
	}

	glm::mat3 OpenGLModel::getNormalMatrix()
	{
		return this->normalMatrix;
	}

	void OpenGLModel::updateMatrices()
	{
		// creates a model matrix with the model position, simple as it looks
		this->modelMatrix = glm::translate(glm::mat4(1.f), this->position);
		// creates a normal matrix with the model matrix
		this->normalMatrix = glm::transpose(glm::inverse(glm::mat3(this->modelMatrix)));
		this->updateCorners();
	}

	void OpenGLModel::translate(glm::vec3 newPos)
	{
		if (this->position == newPos) return;
		this->position = newPos;
		this->updateMatrices();
	}

	bool OpenGLModel::isCamInsideModel(Camera* cam)
	{
		return this->corners[0].x <= cam->pos.x && cam->pos.x <= this->corners[7].x &&
			   this->corners[0].y <= cam->pos.x && cam->pos.y <= this->corners[7].y &&
			   this->corners[0].z <= cam->pos.x && cam->pos.z <= this->corners[7].z;
	}

	bool OpenGLModel::isInsideClippingPlane(Camera* cam)
	{
		if (this->isCamInsideModel(cam)) return true;
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
	

	void OpenGLModel::updateCorners()
	{
		this->corners[0] = this->position;
		this->corners[1] = glm::vec3(this->position.x + this->mesh->size.x, this->position.y, this->position.z);
		this->corners[2] = glm::vec3(this->position.x, this->position.y + this->mesh->size.y, this->position.z);
		this->corners[3] = glm::vec3(this->position.x, this->position.y, this->position.z + this->mesh->size.z);
		this->corners[4] = glm::vec3(this->position.x + this->mesh->size.x, this->position.y + this->mesh->size.y, this->position.z);
		this->corners[5] = glm::vec3(this->position.x + this->mesh->size.x, this->position.y, this->position.z + this->mesh->size.z);
		this->corners[6] = glm::vec3(this->position.x, this->position.y + this->mesh->size.y, this->position.z + this->mesh->size.z);
		this->corners[7] = glm::vec3(this->position.x + this->mesh->size.x, this->position.y + this->mesh->size.y, this->position.z + this->mesh->size.z);
	}


}

