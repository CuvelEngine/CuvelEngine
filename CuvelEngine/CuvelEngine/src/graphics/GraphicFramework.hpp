#pragma once
#include <GLFW/glfw3.h>

namespace cuvel
{
	//Interface for the Graphics framework, will be inherited by each graphics library implementation
	class GraphicFramework
	{
	public:
		virtual void update() = 0;
		virtual void event() = 0;
		virtual void render() = 0;

		int isWindowClosing();

	protected:
		bool createWindow();

		GLFWwindow* window = nullptr;
	};
}