#include "stdafx.h"
#include "VulkanFramework.hpp"

#include "utils.hpp"

namespace cuvel
{

	VulkanFramework::VulkanFramework()
	{
		// Initialize vk context
		this->context = vk::raii::Context();

		// Create vk instance
		vk::ApplicationInfo applicationInfo(this->name.c_str(), 1, this->name.c_str(), 1, VK_API_VERSION_1_3);
		vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);
		this->instance = vk::raii::Instance(this->context, instanceCreateInfo);

		// Obtain physical device. We may have to look for specific extensions in devices in the future
		// Cough cough mesh shaders cough cough.
		// But for now we get the first one we can find
		this->pDevice = vk::raii::PhysicalDevices(instance).front();

		// Get GPU queue family
		uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(this->pDevice.getQueueFamilyProperties());

		// Create windows
		if (!VulkanFramework::initglfw())
		{
			throw std::exception("Error initiating GLFW");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		//TODO: Allow resizing the window
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		if (!this->createWindow())
		{
			throw std::exception("Error creating a window with GLFW");
		}

		// Create surface
		VkSurfaceKHR _surface;
		glfwCreateWindowSurface(*instance, this->window, nullptr, &_surface);
		this->surface = vk::raii::SurfaceKHR(instance, _surface);

		uint32_t presentQueueFamilyIndex = findPresentQueueFamilyIndex(
			this->pDevice, 
			graphicsQueueFamilyIndex, 
			this->surface, 
			this->pDevice.getQueueFamilyProperties()
		);

		// Get logical device
		float queuePriority = 0.0f;
		vk::DeviceQueueCreateInfo deviceQueueCreateInfo({}, graphicsQueueFamilyIndex, 1, &queuePriority);
		vk::DeviceCreateInfo deviceCreateInfo({}, deviceQueueCreateInfo);
		this->lDevice = makeDevice(this->pDevice, graphicsQueueFamilyIndex, { VK_KHR_SWAPCHAIN_EXTENSION_NAME });

		// ******* INIT SWAPCHAIN IN VULKAN HPP *******

		// Get command pool
		vk::CommandPoolCreateInfo commandPoolCreateInfo({}, graphicsQueueFamilyIndex);
		this->commandPool = vk::raii::CommandPool(this->lDevice, commandPoolCreateInfo);

		// Getcommand buffer
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo(*commandPool, vk::CommandBufferLevel::ePrimary, 1);
		this->commandBuffer = std::move(vk::raii::CommandBuffers(this->lDevice, commandBufferAllocateInfo).front());
	}

	VulkanFramework::~VulkanFramework()
	{

	}

	void VulkanFramework::update(float_t& dt)
	{

	}

	void VulkanFramework::event(cuvel::KeyMapper* keyMapper, float_t& dt)
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
