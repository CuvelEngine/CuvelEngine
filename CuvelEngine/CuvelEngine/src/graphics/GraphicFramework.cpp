#include "GraphicFramework.hpp"

#include <fstream>
#include <iostream>

namespace cuvel
{
    bool GraphicFramework::initglfw()
    {
        if (!glfwInit())
            return false;
        return true;
    }

    bool GraphicFramework::createWindow()
    {

        this->window = glfwCreateWindow(1280, 720, "Cuvel Engine", NULL, NULL);
        if (!this->window)
        {
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        return true;
    }

    void GraphicFramework::initProjection(bool invertedY)
    {
        this->camera.updateViewMatrix();

        // Papa GLM does everything for us :D
    	glm::mat4 ProjectionMatrix(1.0f);
        this->projMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(this->fbwidth) / this->fbheight, NEAR_PLANE, FAR_PLANE);
    }

    void GraphicFramework::setLockCursor(const bool lock)
    {
        if (lock)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    std::string GraphicFramework::loadShaderSrc(const std::string& file)
    {
        std::string src;
        std::fstream inFile;

        inFile.open(file);
        if (inFile.is_open())
        {
            std::string temp;
            while (std::getline(inFile, temp))
                src += temp + "\n";
        }
        else
        {
            std::cout << "Could not open " << file << " file" << std::endl;
            throw std::exception("Error loading shader");
        }
        inFile.close();
        return src;
    }

    // Destroy the window. This is called when children are destroyed too so it's perfect
    GraphicFramework::~GraphicFramework()
    {
        std::cout << "Called parent destructor!!";
        glfwTerminate();
    }

    // This basic function will use glfw to get keyboard inputs and update mouse inputs
    void GraphicFramework::event(const float_t& dt)
    {
        glfwPollEvents();

        // Simply go through each relevant key and edit the camera if it is triggered
        // TODO: Improve for greater flexibility
        if (glfwGetKey(window, FORW) == GLFW_PRESS)
        {
            this->camera.updateKeyboardInput(dt, Directions::forward);
        }
        if (glfwGetKey(window, BACKW) == GLFW_PRESS)
        {
            this->camera.updateKeyboardInput(dt, Directions::backward);
        }
        if (glfwGetKey(window, LEFT) == GLFW_PRESS)
        {
            this->camera.updateKeyboardInput(dt, Directions::left);
        }
        if (glfwGetKey(window, RIGHT) == GLFW_PRESS)
        {
            this->camera.updateKeyboardInput(dt, Directions::right);
        }
        if (glfwGetKey(window, UP) == GLFW_PRESS)
        {
            this->camera.updateKeyboardInput(dt, Directions::up);
        }
        if (glfwGetKey(window, DOWN) == GLFW_PRESS)
        {
            this->camera.updateKeyboardInput(dt, Directions::down);
        }
        if (glfwGetKey(window, CURSOR) == GLFW_PRESS && this->isCursorReleased)
        {
            this->isMouseLocked = !this->isMouseLocked;
            this->setLockCursor(this->isMouseLocked);
            this->isCursorReleased = false;
        }
        if (glfwGetKey(window, CURSOR) == GLFW_RELEASE && !this->isCursorReleased)
        {
            this->isCursorReleased = true;
        }

        if (this->isMouseLocked)
        {
            // Update the mouse, it takes the raw mouse data and handles it internally as needed
            double mouseX;
            double mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            this->camera.updateMouseInput(dt, mouseX, mouseY);
        }
        else 
        {
            this->camera.firstMouse = true;
        }

        // Instead of updating after every change it just updates it manually at the end
        this->camera.updateViewMatrix();
    }

    // Detect if the cross at the top right of the window has been pressed.
    int GraphicFramework::isWindowClosing()
    {
        return glfwWindowShouldClose(window);
    }
}

