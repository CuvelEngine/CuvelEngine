#include "stdafx.h"
#include "VulkanFramework.hpp"

namespace cuvel
{

    VulkanFramework::VulkanFramework()
    {
    }

    VulkanFramework::~VulkanFramework()
    {

    }

    void VulkanFramework::update(float_t & dt)
    {

    }

    void VulkanFramework::event(cuvel::KeyMapper * keyMapper, float_t & dt)
    {
        GraphicFramework::event(keyMapper, dt);
    }

    void VulkanFramework::render()
    {

    }

    void VulkanFramework::addModel(uint32_t id, cuvel::Mesh mesh, bool hasLighting, glm::vec3 pos)
    {

    }

    void VulkanFramework::setupImgui()
    {

    }

    void VulkanFramework::newFrameImgui()
    {

    }

    void VulkanFramework::destroyImgui()
    {

    }

    void VulkanFramework::imguiWindow()
    {

    }
}
