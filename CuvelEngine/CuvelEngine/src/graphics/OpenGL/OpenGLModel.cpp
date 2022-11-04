#include "OpenGLModel.hpp"

namespace cuvel
{
	OpenGLModel::OpenGLModel(Mesh mesh, const uint32_t coreProgram, const bool hasLighting)
	{
		// Simply move the given mesh into the model
		this->mesh = std::move(mesh);
		this->coreProgram = coreProgram;
		this->hasLighting = hasLighting ? 1 : 0;

		// Tell OpenGL to create 1 vertexArray
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Tell OpenGL to create 1 vertexBuffer and bind the mesh data to it
		// STATIC selection since for now we won't be changing the meshes actively
		// Might have to be changed to DYNAMIC
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->mesh.vertices.size() * sizeof(Vertex), this->mesh.vertices.data(), GL_STATIC_DRAW);

		// Tell OpenGL to create an index buffer and bind the mesh data to it
		// Again, STATIC might need to change
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh.indices.size() * sizeof(uint32_t), this->mesh.indices.data(), GL_STATIC_DRAW);

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
		this->mesh.updateMatrices();
	}

	OpenGLModel::~OpenGLModel()
	{
		// DELET THIS
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void OpenGLModel::loadUniform()
	{
		// Update the model matrix uniform for this model
		glUniformMatrix4fv(glGetUniformLocation(this->coreProgram, "ModelMatrix"), 1, false, glm::value_ptr(this->mesh.getModelMatrix()));

		// Update the normal matrix as well
		glUniformMatrix3fv(glGetUniformLocation(this->coreProgram, "NormalMatrix"), 1, false, glm::value_ptr(this->mesh.getNormalMatrix()));

		// Update the lighting flag
		glUniform1ui(glGetUniformLocation(this->coreProgram, "hasLighting"), this->hasLighting);

	}

	void OpenGLModel::render()
	{
		this->loadUniform();
		// So what this does is it selects the array we want to render then say "pls drow in scrin"
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, this->mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLModel::getRenderStats(uint32_t* vertices, uint32_t* indices)
	{
		*vertices += this->mesh.vertices.size();
		*indices += this->mesh.indices.size();
	}
	void OpenGLModel::translate(const glm::vec3 newPos)
	{
		if (this->mesh.position == newPos) return;
		this->mesh.position = newPos;
		this->mesh.updateMatrices();
	}
}
