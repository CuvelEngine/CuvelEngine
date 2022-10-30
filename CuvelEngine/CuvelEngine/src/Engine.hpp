#pragma once
#include "graphics/GraphicFramework.hpp"
#include "imgui/ImguiManager.hpp"

// Main class of the system. It literally just initiates everything then run the loop
// Doesn't do much for now tbh
class Engine
{
public:
	explicit Engine(GLibrary lib);
	~Engine();
	void run();

	void addWindows();

	float dt;
private:
	void updateDt();

	float curTime;
	float lastTime;

	cuvel::GraphicFramework* gFramework;
	cuvel::ImguiManager* imguiManager;

	//imgui support flags
};

