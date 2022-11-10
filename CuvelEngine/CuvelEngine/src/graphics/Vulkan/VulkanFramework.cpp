#include "VulkanFramework.hpp"

#include <iostream>
#include <set>

#include "imgui/backends/imgui_impl_vulkan.h"
#include "imgui/backends/imgui_impl_glfw.h"

static ImGui_ImplVulkanH_Window g_MainWindowData;
static int                      g_MinImageCount = 2;
static bool                     g_SwapChainRebuild = false;

static void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[Imgui - Vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}


struct SwapChainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
	void* /*pUserData*/)
{
	std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
		<< vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
	std::cerr << std::string("\t") << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
	std::cerr << std::string("\t") << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
	std::cerr << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
	if (0 < pCallbackData->queueLabelCount)
	{
		std::cerr << std::string("\t") << "Queue Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
		{
			std::cerr << std::string("\t\t") << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
		}
	}
	if (0 < pCallbackData->cmdBufLabelCount)
	{
		std::cerr << std::string("\t") << "CommandBuffer Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
		{
			std::cerr << std::string("\t\t") << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
		}
	}
	if (0 < pCallbackData->objectCount)
	{
		std::cerr << std::string("\t") << "Objects:\n";
		for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
		{
			std::cerr << std::string("\t\t") << "Object " << i << "\n";
			std::cerr << std::string("\t\t\t") << "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType))
				<< "\n";
			std::cerr << std::string("\t\t\t") << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
			if (pCallbackData->pObjects[i].pObjectName)
			{
				std::cerr << std::string("\t\t\t") << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
			}
		}
	}
	return VK_TRUE;
}

vk::DebugUtilsMessengerCreateInfoEXT makeDebugUtilsMessengerCreateInfoEXT()
{
	return { {},
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
		&debugUtilsMessengerCallback };
}

namespace cuvel
{

	void VulkanFramework::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto vkFramework = static_cast<VulkanFramework*>(glfwGetWindowUserPointer(window));
		vkFramework->framebufferResized = true;
		vkFramework->width = width;
		vkFramework->height = height;
	}

	bool VulkanFramework::checkValidationLayerSupport()
	{
		std::vector<vk::ExtensionProperties> props = vk::enumerateInstanceExtensionProperties();
		auto propertyIterator = std::ranges::find_if(
			props, 
			[](vk::ExtensionProperties const& ep)
			{
				return strcmp(ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0;
			});
		return !(propertyIterator == props.end());
	}

	void VulkanFramework::createInstance()
	{
		// Check for layers
		if (enableValidationLayers && !this->checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		// Create information struct for the instance
		vk::ApplicationInfo applicationInfo(this->name.c_str(), 1, this->name.c_str(), 1, VK_API_VERSION_1_3);
		auto instanceCreateInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&applicationInfo,
			0, nullptr, // enabled layers
			static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
		);

		// If in debug mode we enable validation layers
		if (enableValidationLayers) {
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}

		// Create instance
		this->instance = vk::raii::Instance(context, instanceCreateInfo);

	}

	void VulkanFramework::createDebugCallback()
	{
		this->debugUtils = vk::raii::DebugUtilsMessengerEXT(instance, makeDebugUtilsMessengerCreateInfoEXT());
	}

	void VulkanFramework::createSurface()
	{
		// Create surface
		VkSurfaceKHR _surface;
		glfwCreateWindowSurface(*instance, window, nullptr, &_surface);
		this->surface = vk::raii::SurfaceKHR(instance, _surface);

	}

	void VulkanFramework::choosePhysicalDevice()
	{
		// Get list of GPUs that we could use
		vk::raii::PhysicalDevices physicalDevices(instance);

		// Look for one that works
		bool found = false;
		for (vk::raii::PhysicalDevice pDevice : physicalDevices)
		{
			this->familyIndices = {};
			auto queueFamilies = pDevice.getQueueFamilyProperties();

			int i = 0;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
					this->familyIndices.graphicsFamily = i;
				}

				if (queueFamily.queueCount > 0 && pDevice.getSurfaceSupportKHR(i, *surface)) {
					this->familyIndices.presentFamily = i;
				}

				if (this->familyIndices.isComplete()) {
					break;
				}

				i++;
			}
			std::set<std::string> requiredExtensions(this->deviceExtensions.begin(), this->deviceExtensions.end());
			for (const auto& extension : pDevice.enumerateDeviceExtensionProperties()) {
				requiredExtensions.erase(extension.extensionName);
			}
			bool extensionsSupported = requiredExtensions.empty();

			bool swapChainAdequate = false;
			if (extensionsSupported) {
				SwapChainSupportDetails swapChainSupport;
				swapChainSupport.capabilities = pDevice.getSurfaceCapabilitiesKHR(*surface);
				swapChainSupport.formats = pDevice.getSurfaceFormatsKHR(*surface);
				swapChainSupport.presentModes = pDevice.getSurfacePresentModesKHR(*surface);
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}
			if (this->familyIndices.isComplete() && extensionsSupported && swapChainAdequate)
			{
				this->pDevice = vk::raii::PhysicalDevice(std::move(pDevice));
				found = true;
				break;
			}
		}
		if (!found)
		{
			throw std::runtime_error("Could not found any suitable GPU");
		}
	}

	void VulkanFramework::createLogicalDevice()
	{

		// Pick queue family data
		float queuePriority = 0.0f;
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {
			this->familyIndices.graphicsFamily.value(),
			this->familyIndices.presentFamily.value()
		};
		queueCreateInfos.reserve(uniqueQueueFamilies.size());
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags(),
				queueFamily, 1, &queuePriority
			);
		}

		// Create logical device
		vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(), queueCreateInfos.size(), queueCreateInfos.data());
		deviceCreateInfo.setPEnabledExtensionNames(this->deviceExtensions);
		this->lDevice = vk::raii::Device(this->pDevice, deviceCreateInfo);
	}

	void VulkanFramework::createCommandPool()
	{
		// create a CommandPool to allocate a CommandBuffer from
		vk::CommandPoolCreateInfo commandPoolCreateInfo({}, this->familyIndices.graphicsFamily.value());
		this->commandPool = vk::raii::CommandPool(this->lDevice, commandPoolCreateInfo);

	}

	void VulkanFramework::createSwapChain()
	{
		// get the supported VkFormats
		std::vector<vk::SurfaceFormatKHR> formats = this->pDevice.getSurfaceFormatsKHR(*surface);
		assert(!formats.empty());
		this->format = formats[0].format == vk::Format::eUndefined ? vk::Format::eB8G8R8A8Unorm : formats[0].format;

		vk::SurfaceCapabilitiesKHR surfaceCapabilities = this->pDevice.getSurfaceCapabilitiesKHR(*surface);
		if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
		{
			// If the surface size is undefined, the size is set to the size of the images requested.
			this->swapchainExtent.width = std::clamp<uint32_t>(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
			this->swapchainExtent.height = std::clamp<uint32_t>(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			this->swapchainExtent = surfaceCapabilities.currentExtent;
		}

		std::vector<vk::PresentModeKHR> presentModes = this->pDevice.getSurfacePresentModesKHR(*surface);
		auto finalPresentMode = vk::PresentModeKHR::eImmediate;
		bool found = false;
		for (const auto& desiredPresentMode : this->swapChainModePriorities) {
			if (found) break;
			for (const auto& availablePresentMode : presentModes)
			{
				if (static_cast<vk::PresentModeKHR>(desiredPresentMode) == availablePresentMode)
				{
					finalPresentMode = static_cast<vk::PresentModeKHR>(desiredPresentMode);
					found = true;
					break;
				}
			}
		}

		vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
			? vk::SurfaceTransformFlagBitsKHR::eIdentity
			: surfaceCapabilities.currentTransform;

		vk::CompositeAlphaFlagBitsKHR compositeAlpha =
			(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
			: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
			: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit
			: vk::CompositeAlphaFlagBitsKHR::eOpaque;

		vk::SwapchainCreateInfoKHR swapChainCreateInfo(vk::SwapchainCreateFlagsKHR(), *surface, surfaceCapabilities.minImageCount,
			this->format, vk::ColorSpaceKHR::eSrgbNonlinear, this->swapchainExtent, 1, vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive, {}, preTransform, compositeAlpha, finalPresentMode, true, nullptr);

		std::array<uint32_t, 2> queueFamilyIndices = { this->familyIndices.graphicsFamily.value(), this->familyIndices.presentFamily.value() };
		if (this->familyIndices.graphicsFamily.value() != this->familyIndices.presentFamily.value())
		{
			// If the graphics and present queues are from different queue families, we either have to explicitly transfer
			// ownership of images between the queues, or we have to create the swapchain with imageSharingMode as
			// VK_SHARING_MODE_CONCURRENT
			swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			swapChainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
			swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}

		// Create swapchain
		this->swapChain = this->lDevice.createSwapchainKHR(swapChainCreateInfo);
	}

	void VulkanFramework::createImageViews()
	{
		// Create image views
		std::vector<vk::Image> swapChainImages = swapChain.getImages();
		this->swapChainImageViews.reserve(swapChainImages.size());
		vk::ImageViewCreateInfo imageViewCreateInfo({}, {}, vk::ImageViewType::e2D, static_cast<vk::Format>(format), {}, { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });
		for (auto& image : swapChainImages)
		{
			imageViewCreateInfo.image = image;
			swapChainImageViews.emplace_back(this->lDevice, imageViewCreateInfo);
		}

	}

	void VulkanFramework::createCommandBuffers()
	{
		// allocate a CommandBuffer from the CommandPool
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo(*this->commandPool, vk::CommandBufferLevel::ePrimary, this->framesInFlight);
		this->commandBuffers = vk::raii::CommandBuffers(this->lDevice, commandBufferAllocateInfo);
	}

	void VulkanFramework::createRenderPass()
	{
		auto formats = this->pDevice.getSurfaceFormatsKHR(*this->surface);
		vk::SurfaceFormatKHR pickedFormat = formats[0];
		if (formats.size() == 1)
		{
			if (formats[0].format == vk::Format::eUndefined)
			{
				pickedFormat.format = vk::Format::eB8G8R8A8Unorm;
				pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
			}
		}
		else
		{
			// request several formats, the first found will be used
			vk::Format requestedFormats[] = { vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm };
			vk::ColorSpaceKHR requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
			for (auto requestedFormat : requestedFormats)
			{
				auto it = std::ranges::find_if(formats,
					[requestedFormat, requestedColorSpace](vk::SurfaceFormatKHR const& f)
					{ return (f.format == requestedFormat) && (f.colorSpace == requestedColorSpace); });
				if (it != formats.end())
				{
					pickedFormat = *it;
					break;
				}
			}
		}
		assert(pickedFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);
		vk::Format colorFormat = pickedFormat.format;
		vk::Format depthFormat = this->depthFormat;
		std::array<vk::AttachmentDescription, 2> attachmentDescriptions;
		attachmentDescriptions[0] = vk::AttachmentDescription({},
			colorFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR);
		attachmentDescriptions[1] = vk::AttachmentDescription({},
			depthFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal);

		vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
		vk::AttachmentReference depthReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
		vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, {}, colorReference, {}, &depthReference);

		vk::RenderPassCreateInfo renderPassCreateInfo({}, attachmentDescriptions, subpass);
		this->renderPass = vk::raii::RenderPass(this->lDevice, renderPassCreateInfo);
	}

	void VulkanFramework::createDepthResources()
	{
		vk::ImageTiling tiling;
		bool found = false;
		for (auto& format : this->desiredFormats)
		{
			vk::FormatProperties formatProperties = this->pDevice.getFormatProperties(format);
			if (formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
			{
				this->depthFormat = format;
				tiling = vk::ImageTiling::eLinear;
				found = true;
				break;
			}
			if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
			{
				this->depthFormat = format;
				tiling = vk::ImageTiling::eOptimal;
				found = true;
				break;
			}
		}
		if (!found)
		{
			throw std::runtime_error("DepthStencilAttachment is not supported for any desired format");
		}

		vk::ImageCreateInfo imageCreateInfo({},
			vk::ImageType::e2D,
			this->depthFormat,
			vk::Extent3D(this->swapchainExtent, 1),
			1,
			1,
			vk::SampleCountFlagBits::e1,
			tiling,
			vk::ImageUsageFlagBits::eDepthStencilAttachment);
		vk::raii::Image depthImage(this->lDevice, imageCreateInfo);

		vk::PhysicalDeviceMemoryProperties memoryProperties = this->pDevice.getMemoryProperties();
		vk::MemoryRequirements memoryRequirements = depthImage.getMemoryRequirements();

		uint32_t typeBits = memoryRequirements.memoryTypeBits;
		uint32_t typeIndex = static_cast<uint32_t>(~0);
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if ((typeBits & 1) &&
				((memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) == vk::MemoryPropertyFlagBits::eDeviceLocal))
			{
				typeIndex = i;
				break;
			}
			typeBits >>= 1;
		}
		assert(typeIndex != uint32_t(~0));

		vk::MemoryAllocateInfo memoryAllocateInfo(memoryRequirements.size, typeIndex);
		vk::raii::DeviceMemory depthMemory(this->lDevice, memoryAllocateInfo);
		depthImage.bindMemory(*depthMemory, 0);

		vk::ImageViewCreateInfo imageViewCreateInfo({}, *depthImage, vk::ImageViewType::e2D, depthFormat, {}, { vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 });
		this->depthImageView = vk::raii::ImageView(this->lDevice, imageViewCreateInfo);

	}

	void VulkanFramework::createFrameBuffers()
	{
		std::array<vk::ImageView, 2> attachments;
		attachments[1] = *this->depthImageView;

		this->framebuffers.reserve(this->swapChainImageViews.size());
		for (auto const& view : this->swapChainImageViews)
		{
			attachments[0] = *view;
			vk::FramebufferCreateInfo framebufferCreateInfo({}, *renderPass, attachments, this->swapchainExtent.width, this->swapchainExtent.height, 1);
			this->framebuffers.emplace_back(this->lDevice, framebufferCreateInfo);
		}
	}

	void VulkanFramework::createSyncObjects()
	{
		imageAvailableSemaphores.reserve(this->framesInFlight);
		renderFinishedSemaphores.reserve(this->framesInFlight);
		inFlightFences.reserve(this->framesInFlight);
		imagesInFlight.reserve(this->swapChainImageViews.size());
		
		for (size_t i = 0; i < this->framesInFlight; i++) {
			vk::SemaphoreCreateInfo semaphoreInfo;
			imageAvailableSemaphores.emplace_back(this->lDevice, semaphoreInfo);
			renderFinishedSemaphores.emplace_back(this->lDevice, semaphoreInfo);

			vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);
			inFlightFences.emplace_back(this->lDevice, fenceInfo);
		}
	}

	VulkanFramework::VulkanFramework()
	{
		if (!cuvel::VulkanFramework::initglfw())
		{
			throw std::exception("Error initiating GLFW");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		if (!this->createWindow())
		{
			throw std::exception("Error creating a window");
		}

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

		this->createInstance();
		if (this->enableValidationLayers) this->createDebugCallback();
		this->createSurface();
		this->choosePhysicalDevice();
		this->createLogicalDevice();
		this->createCommandPool();
		this->createSwapChain();
		this->createImageViews();
		this->createDepthResources();
		this->createRenderPass();
		this->createFrameBuffers();
		this->createCommandBuffers();
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
		ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = *this->instance;
		init_info.PhysicalDevice = *this->pDevice;
		init_info.Device = *this->lDevice;
		init_info.QueueFamily = this->familyIndices.graphicsFamily.value();
		init_info.Queue = VK_NULL_HANDLE;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = VK_NULL_HANDLE;
		init_info.Subpass = 0;
		init_info.MinImageCount = g_MinImageCount;
		init_info.ImageCount = wd->ImageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = VK_NULL_HANDLE;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
	}

	void VulkanFramework::newFrameImgui()
	{
		if (g_SwapChainRebuild)
		{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			if (width > 0 && height > 0)
			{
				ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
				ImGui_ImplVulkanH_CreateOrResizeWindow(
					*this->instance, 
					*this->pDevice, 
					*this->lDevice, 
					&g_MainWindowData, 
					this->familyIndices.graphicsFamily.value(), 
					VK_NULL_HANDLE, 
					width, height, 
					g_MinImageCount);
				g_MainWindowData.FrameIndex = 0;
				g_SwapChainRebuild = false;
			}
		}
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void VulkanFramework::destroyImgui()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void VulkanFramework::imguiWindow()
	{

	}
}
