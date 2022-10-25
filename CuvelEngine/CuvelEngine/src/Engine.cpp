#include "Engine.hpp"

#include <exception>

#include "graphics/OpenGL/OpenGLFramework.hpp"

Engine::Engine(const GLibrary lib)
{
	// This should be able to do hot reload in the future!!
	if (lib == GLibrary::OpenGL)
	{
		this->gFramework = new cuvel::OpenGLFramework();
	}
	else if (lib == GLibrary::Vulkan)
	{
		//TODO: Implementar vulkan
		throw std::exception("Vulkan is not implemented");
	}

	// Im just testing a triangle, might delete later UwU
	cuvel::Mesh mesh{};
	mesh.vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
	mesh.vertices.push_back({ glm::vec3( 0.5f, -0.5f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
	mesh.vertices.push_back({ glm::vec3(  0.f,  0.5f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f) });
	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	this->gFramework->addModel(0, mesh);
}

Engine::~Engine()
{
	// DELETE THIS
	delete this->gFramework;
	glfwTerminate();
}

void Engine::run()
{
	// Each loop is one frame
	while (!this->gFramework->isWindowClosing())
	{
		this->updateDt();
		this->gFramework->event(this->dt);
		this->gFramework->update(this->dt);
		this->gFramework->render();
	}
}

void Engine::updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	dt = curTime - lastTime;
	lastTime = curTime;
}
