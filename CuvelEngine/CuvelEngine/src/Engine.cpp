#include "Engine.hpp"

#include <exception>

#include "graphics/OpenGL/OpenGLFramework.hpp"
#include "imgui/ImguiManager.hpp"
#include "movement/KeyMapper.hpp"
#include <iostream>

#include "voxel/Voxel.hpp"

#include <chrono>

Engine::Engine(GLibrary lib): dt(0), curTime(0), lastTime(0)
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

	std::chrono::time_point<std::chrono::high_resolution_clock> start, mid, mid2, end;
	start = std::chrono::high_resolution_clock::now();

	// Load the voxm file
	std::string filename = "assets/testFinal.voxm";
	cuvel::VoxelModel voxModel(filename);

	mid = std::chrono::high_resolution_clock::now();

	// Generate the mesh
	std::shared_ptr<cuvel::Mesh> mesh = this->gFramework->addMesh(filename);
	voxModel.populateMesh(mesh);
	this->gFramework->registerMeshBuffers(filename);

	mid2 = std::chrono::high_resolution_clock::now();

	// Load the models
	int count = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			this->gFramework->addModel(count, filename, x % 2 == 0, { x * 255, 0, y * 255 });
			count++;
		}
	}

	end = std::chrono::high_resolution_clock::now();

	// End result
	std::chrono::duration<double> step1 = mid - start;
	std::cout << "Read voxm in " << step1.count() << " seconds" << std::endl;
	std::chrono::duration<double> step2 = mid2 - mid;
	std::cout << "Generated mesh in " << step2.count() << " seconds" << std::endl;
	std::chrono::duration<double> step3 = end - mid2;
	std::cout << "Created models in " << step3.count() << " seconds" << std::endl;
	std::chrono::duration<double> total = end - start;
	std::cout << "Total time to load scene taken: " << total.count() << " seconds" << std::endl;
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
