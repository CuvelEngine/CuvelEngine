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
        //TODO: Prepare for Vulkan (Vulkan uses inverted y axis)
        this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        this->camFront = glm::vec3(0.0f, 0.0f, -1.0f);
        this->cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
        this->viewMatrix = glm::mat4(1.0f);

        // Papa GLM does everything for us :D
        this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->camFront, this->worldUp);
        glm::mat4 ProjectionMatrix(1.0f);
        this->projMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(this->fbwidth) / this->fbheight, NEAR_PLANE, FAR_PLANE);
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

    int GraphicFramework::isWindowClosing()
    {
        return glfwWindowShouldClose(window);
    }
}

