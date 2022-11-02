#include "GraphicFramework.hpp"

#include <fstream>
#include <iostream>

#include "movement/KeyMapper.hpp"

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

        this->window = glfwCreateWindow(1280, 720, "Cuvel Engine", nullptr, nullptr);
        if (!this->window)
        {
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);

        //This line of code disables VSync
        //glfwSwapInterval(0);
        return true;
    }

    void GraphicFramework::initProjection(bool invertedY)
    {
        this->camera.updateViewMatrix();

        // Papa GLM does everything for us :D
    	glm::mat4 ProjectionMatrix(1.0f);
        this->projMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(this->fbwidth) / this->fbheight, NEAR_PLANE, FAR_PLANE);
    }

    void GraphicFramework::setLockCursor()
    {
        const int cursorState = glfwGetInputMode(this->window, GLFW_CURSOR);
        if (cursorState == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            this->camera.firstMouse = true;
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
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
        glfwTerminate();
    }

    // This basic function will use glfw to get keyboard inputs and update mouse inputs
    void GraphicFramework::event(KeyMapper* keyMapper, const float_t& dt)
    {
        glfwPollEvents();

        keyMapper->executeKeyMaps(dt);

        const int cursorState = glfwGetInputMode(this->window, GLFW_CURSOR);
        if (cursorState == GLFW_CURSOR_DISABLED)
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

    bool GraphicFramework::isKeyPressed(const int key)
    {
        return glfwGetKey(this->window, key) == GLFW_PRESS;
    }
}

