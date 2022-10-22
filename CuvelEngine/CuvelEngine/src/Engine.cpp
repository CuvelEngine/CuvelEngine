#include "Engine.hpp"

#include <exception>

#include "graphics/OpenGL/OpenGLFramework.hpp"

Engine::Engine(const GLibrary lib)
{
	if (lib == GLibrary::OpenGL)
	{
		this->gFramework = new cuvel::OpenGLFramework();
	}
	else if (lib == GLibrary::Vulkan)
	{
		//TODO: Implementar vulkan
		throw std::exception("Vulkan is not implemented");
	}
}

Engine::~Engine()
{
	delete this->gFramework;
}

void Engine::run()
{
	while (!this->gFramework->isWindowClosing())
	{
		this->gFramework->update();
		this->gFramework->render();
	}
}
