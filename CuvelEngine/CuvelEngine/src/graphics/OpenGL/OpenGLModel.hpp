#pragma once
#include <cstdint>

#include "graphics/Mesh.hpp"

namespace cuvel
{
	// Basic class for the OpenGL Model objects.
	// It simply contains the mesh and the data OpenGL needs to work.
	// Nothing special
	class OpenGLModel
	{
	public:
		explicit OpenGLModel(Mesh mesh, uint32_t coreProgram, bool hasLighting);
		~OpenGLModel();

		void render();

		// The mesh with the buffer data
		Mesh mesh;
	private:
		void loadUniform();
		// Buffer and Array IDs from OpenGL
		uint32_t VAO{};
		uint32_t VBO{};
		uint32_t EBO{};

		// The shader from the framework
		uint32_t coreProgram;
		uint8_t hasLighting;
	};
}
