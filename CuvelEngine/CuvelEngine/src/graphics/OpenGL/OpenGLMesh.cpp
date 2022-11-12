#include "OpenGLMesh.hpp"

#include "settings/GraphicOptions.hpp"

namespace cuvel
{
	void OpenGLMesh::render()
	{
		// Select the array we want to render and issue a draw call to the GPU
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLMesh::registerBuffers(uint32_t coreProgram)
	{
		// Tell OpenGL to create 1 vertexArray
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Tell OpenGL to create 1 vertexBuffer and bind the mesh data to it
		// STATIC selection since for now we won't be changing the meshes actively
		// Might have to be changed to DYNAMIC
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

		// Tell OpenGL to create an index buffer and bind the mesh data to it
		// Again, STATIC might need to change
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(uint32_t), this->indices.data(), GL_STATIC_DRAW);

		// Define the vertex position. Simply get the shader attribute location  and
		// bind it to the location of the position inside of the Vertex structure
		GLuint attribLoc = glGetAttribLocation(coreProgram, "position");
		glVertexAttribPointer(attribLoc, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(attribLoc);

		// Same thing with color. This time it's 4 unsigned bytes, and the GL_TRUE is signed
		// because we want to have it normalized
		attribLoc = glGetAttribLocation(coreProgram, "color");
		glVertexAttribPointer(attribLoc, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
		glEnableVertexAttribArray(attribLoc);

		// We finally add the normal index
		attribLoc = glGetAttribLocation(coreProgram, "normalIndex");
		glVertexAttribIPointer(attribLoc, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(attribLoc);
	}

	OpenGLMesh::~OpenGLMesh()
	{
		// We clean the buffers
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}
