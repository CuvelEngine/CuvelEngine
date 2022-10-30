#include "Engine.hpp"

#include <exception>

#include "graphics/OpenGL/OpenGLFramework.hpp"
#include "imgui/ImguiManager.hpp"

Engine::Engine(const GLibrary lib): dt(0), curTime(0), lastTime(0)
{
	this->imguiManager = new cuvel::ImguiManager();

	// This should be able to do hot reload in the future!!
	switch (lib)
	{
	case GLibrary::OpenGL:
		this->gFramework = new cuvel::OpenGLFramework();
		break;
	case GLibrary::Vulkan:
		//TODO: Implement vulkan
		throw std::exception("Vulkan is not implemented");
	default:
		throw std::exception("Graphics library not recognized");
	}
	
	this->gFramework->setupImgui();

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
	this->gFramework->destroyImgui();
	delete this->imguiManager;
	delete this->gFramework;
}

void Engine::run()
{
	// Each loop is one frame
	while (!this->gFramework->isWindowClosing())
	{
		this->updateDt();
		this->gFramework->event(this->dt);
		this->gFramework->update(this->dt);
		
		this->processImguiWindows();

		this->gFramework->render();
	}
}

void Engine::updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	dt = curTime - lastTime;
	lastTime = curTime;
}

void Engine::processImguiWindows()
{
	this->gFramework->newFrameImgui();

	this->gFramework->imgui_windows();
	this->gFramework->camera.imgui_windows();

	this->imguiManager->renderImgui();
}
