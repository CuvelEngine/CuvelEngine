#pragma once
#include "graphics/GraphicFramework.hpp"
#include "imgui/ImguiManager.hpp"

// Main class of the system. It literally just initiates everything then run the loop
// Doesn't do much for now tbh
class Engine
{
public:
	explicit Engine();
	~Engine();
	void run();

	void addImguiWindows();
	void addKeyMaps();

	float dt;
private:
	void updateDt();

	float curTime;
	float lastTime;

	cuvel::GraphicFramework* gFramework;
	cuvel::ImguiManager* imguiManager;
	cuvel::KeyMapper* keyMapper;
};

