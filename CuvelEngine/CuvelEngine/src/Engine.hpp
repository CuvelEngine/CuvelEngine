#pragma once
#include "graphics/GraphicFramework.hpp"

enum GLibrary
{
	OpenGL = 0,
	Vulkan = 1
};

class Engine
{
public:
	explicit Engine(GLibrary lib);
	~Engine();
	void run();

private:
	cuvel::GraphicFramework* gFramework;
};

