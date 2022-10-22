#include "GraphicFramework.hpp"

namespace cuvel
{
    bool GraphicFramework::createWindow()
    {
        if (!glfwInit())
            return false;

        this->window = glfwCreateWindow(1280, 720, "Cuvel Engine", NULL, NULL);
        if (!this->window)
        {
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        return true;
    }
    GraphicFramework::~GraphicFramework()
    {
        glfwTerminate();
    }

    int GraphicFramework::isWindowClosing()
    {
        return glfwWindowShouldClose(window);
    }
}

