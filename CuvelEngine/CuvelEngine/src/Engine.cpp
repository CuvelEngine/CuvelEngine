#include "Engine.hpp"

#include <exception>

#include "graphics/OpenGL/OpenGLFramework.hpp"
#include "imgui/ImguiManager.hpp"
#include "movement/KeyMapper.hpp"

Engine::Engine(const GLibrary lib): dt(0), curTime(0), lastTime(0)
{
	// Imgui must be initialized before anything else
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

	// Add context to the KeyMapper so it can interact with the components
	this->keyMapper = new cuvel::KeyMapper(this->gFramework);
	
	this->addImguiWindows();
	this->addKeyMaps();

	// Hook imgui to the specific rendering library
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
	delete this->keyMapper;
}

void Engine::run()
{
	// Each loop is one frame
	while (!this->gFramework->isWindowClosing())
	{
		// Detect input and modify data accordingly
		this->updateDt();
		this->gFramework->event(this->keyMapper, this->dt);
		this->gFramework->update(this->dt);

		// Update imgui
		this->gFramework->newFrameImgui();
		this->imguiManager->renderImgui();

		// Render data in GPU
		this->gFramework->render();
	}
}

void Engine::addImguiWindows()
{
	// Rendering debug window
	this->imguiManager->addWindow(
		"Render",
		this->gFramework,
		{ 10, 30 }, { 250, 120 });

	// Camera controls window
	this->imguiManager->addWindow(
		"Controls",
		&this->gFramework->camera,
		{ 10, 160 }, { 250, 180 });
}

void Engine::addKeyMaps()
{
	// Movement controls
	this->keyMapper->addKeyMap(GLFW_KEY_W, &cuvel::KeyMapper::move, true, "forward");
	this->keyMapper->addKeyMap(GLFW_KEY_S, &cuvel::KeyMapper::move, true, "backward");
	this->keyMapper->addKeyMap(GLFW_KEY_A, &cuvel::KeyMapper::move, true, "left");
	this->keyMapper->addKeyMap(GLFW_KEY_D, &cuvel::KeyMapper::move, true, "right");
	this->keyMapper->addKeyMap(GLFW_KEY_SPACE, &cuvel::KeyMapper::move, true, "up");
	this->keyMapper->addKeyMap(GLFW_KEY_LEFT_CONTROL, &cuvel::KeyMapper::move, true, "down");

	// Press Esc to free the mouse so you can modify the Imgui windows and
	// get out of the program
	this->keyMapper->addKeyMap(GLFW_KEY_ESCAPE, &cuvel::KeyMapper::lockCursor, false);
}

void Engine::updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	dt = curTime - lastTime;
	lastTime = curTime;
}
