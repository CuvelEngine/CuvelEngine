#pragma once
#include "graphics/Mesh.hpp"

#include <vector>


namespace cuvel
{
	// Basic class for the OpenGL Model objects.
	// It simply contains the mesh and the data OpenGL needs to work.
	// Nothing special
	class OpenGLModel
	{
	public:
		explicit OpenGLModel(Mesh mesh, uint32_t coreProgram);
		~OpenGLModel();

		void loadUniform();
		void render();
		void getRenderStats(uint32_t* vertices, uint32_t* indices);

	private:
		// Buffer and Array IDs from OpenGL
		uint32_t VAO{};
		uint32_t VBO{};
		uint32_t EBO{};

		// The shader from the framework
		uint32_t coreProgram;

		// The mesh with the buffer data
		Mesh mesh;
	};
}
