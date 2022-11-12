#pragma once
#include <memory>

#include "graphics/OpenGL/OpenGLMesh.hpp"
#include "movement/Camera.hpp"

namespace cuvel
{
	// Basic class for the OpenGL Model objects.
	// It simply contains the mesh and the data OpenGL needs to work.
	// Nothing special
	class OpenGLModel
	{
	public:
		explicit OpenGLModel(const std::shared_ptr<Mesh>& mesh, uint32_t coreProgram, bool hasLighting);

		void render();
		void getRenderStats(uint32_t* vertices, uint32_t* indices);

		//TODO: Make sure this doesn't flip in Vulkan (Vulkan has inverted y axis)
		glm::mat4 getModelMatrix();
		glm::mat3 getNormalMatrix();
		void updateMatrices();

		void translate(glm::vec3 newPos);
		bool isCamInsideModel(Camera* cam);
		bool isInsideClippingPlane(Camera* cam);

		void updateCorners();
	private:
		void loadUniform();

		// The shader from the framework
		uint32_t coreProgram;
		uint8_t hasLighting;

		// Model data
		glm::vec3 corners[8] = {};
		glm::vec3 position{};
		
		// The mesh with the buffer data
		std::shared_ptr<OpenGLMesh> mesh;

		glm::mat4 modelMatrix{};
		glm::mat3 normalMatrix{};
	};
}
