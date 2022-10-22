#include "OpenGLFramework.hpp"

#include <exception>

namespace cuvel
{
	OpenGLFramework::OpenGLFramework()
	{
		if (!this->createWindow())
		{
			throw std::exception("Error creating a window with GLFW");
		}
	}

	OpenGLFramework::~OpenGLFramework()
	{
		
	}


	void OpenGLFramework::update()
	{

	}

	void OpenGLFramework::event()
	{

	}


	void OpenGLFramework::render()
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}
