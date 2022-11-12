#pragma once
#include "graphics/Mesh.hpp"

namespace cuvel
{
	class OpenGLMesh final : public Mesh
	{
	public:
		void render() override;

		void registerBuffers(uint32_t coreProgram);
		~OpenGLMesh();

	private:
		// Buffer and Array IDs from OpenGL
		uint32_t VAO{};
		uint32_t VBO{};
		uint32_t EBO{};
	};
}


