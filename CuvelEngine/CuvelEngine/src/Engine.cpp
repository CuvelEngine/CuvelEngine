#include "Engine.hpp"

#include "graphics/OpenGL/OpenGLFramework.hpp"
#include "graphics/Vulkan/VulkanFramework.hpp"
#include "movement/KeyMapper.hpp"

Engine::Engine(): dt(0), curTime(0), lastTime(0)
{
	// Imgui must be initialized before anything else
	this->imguiManager = new cuvel::ImguiManager();

	// This should be able to do hot reload in the future!!
#ifdef USE_OPENGL
	this->gFramework = new cuvel::OpenGLFramework();
#endif
#ifdef USE_VULKAN
	this->gFramework = new cuvel::VulkanFramework();
#endif

	// Add context to the KeyMapper so it can interact with the components
	this->keyMapper = new cuvel::KeyMapper(this->gFramework);
	
	this->addImguiWindows();
	this->addKeyMaps();

	// Hook imgui to the specific rendering library
	this->gFramework->setupImgui();

	// Making a cube for testing
	uint32_t count = 0;
	for (uint8_t x = 0; x < 18; x += 3)
	{
		for (uint8_t y = 0; y < 18; y += 3)
		{
			for (uint8_t z = 0; z < 18; z += 3)
			{
				this->gFramework->addModel(count, makeCube(), true, glm::vec3(x, y, z));
				count++;
			}
		}
	}
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
		{ 10, 30 }, { 450, 200 });

	// Camera controls window
	this->imguiManager->addWindow(
		"Controls",
		&this->gFramework->camera,
		{ 10, 240 }, { 450, 180 });
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
