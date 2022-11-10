#pragma once
#include <array>
#include <cmath>
#include <optional>
#include <vulkan/vulkan_raii.hpp>

#include "graphics/GraphicFramework.hpp"


namespace cuvel
{
	enum PresentMode
	{
		direct = vk::PresentModeKHR::eImmediate,
		vSync = vk::PresentModeKHR::eFifo,
		relaxedVSync = vk::PresentModeKHR::eFifoRelaxed,
		tripleBuffer = vk::PresentModeKHR::eMailbox
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class VulkanFramework final : public cuvel::GraphicFramework
	{
	public:
		explicit VulkanFramework();
		~VulkanFramework() override;

		void update(float_t& dt) override;
		void event(cuvel::KeyMapper* keyMapper, float_t& dt) override;
		void render() override;

		void addModel(uint32_t id, cuvel::Mesh mesh, bool hasLighting, glm::vec3 pos) override;

		void setupImgui() override;
		void newFrameImgui() override;
		void destroyImgui() override;

		void imguiWindow() override;

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		bool checkValidationLayerSupport();
		
		void createInstance();
		void createDebugCallback();
		void createSurface();
		void choosePhysicalDevice();
		void createLogicalDevice();
		void createCommandPool();
		void createSwapChain();
		void createImageViews();
		void createCommandBuffers();
		void createRenderPass();
		void createDepthResources();
		void createFrameBuffers();
		void createSyncObjects();

		std::string name = "CuvelEngine - Vulkan";

		//Imgui parameters
		uint32_t vertices = 0;
		uint32_t indices = 0;
		uint16_t drawCalls = 0;

		bool framebufferResized;
		int width;
		int height;

		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		vk::Format depthFormat;
		const std::vector<vk::Format> desiredFormats = {
			vk::Format::eD32SfloatS8Uint,
			vk::Format::eD32Sfloat,
			vk::Format::eD24UnormS8Uint
		};

		QueueFamilyIndices familyIndices;
		vk::Extent2D swapchainExtent;
		std::array<PresentMode, 4> swapChainModePriorities = { tripleBuffer, vSync, relaxedVSync, direct };
		uint32_t framesInFlight = 2;
		vk::Format format;

		vk::raii::Context context{};
		vk::raii::Instance instance = nullptr;
		vk::raii::PhysicalDevice pDevice = nullptr;
		vk::raii::Device lDevice = nullptr;
		vk::raii::CommandPool commandPool = nullptr;
		vk::raii::CommandBuffers commandBuffers = nullptr;
		vk::raii::SurfaceKHR surface = nullptr;
		vk::raii::DebugUtilsMessengerEXT debugUtils = nullptr;
		vk::raii::SwapchainKHR swapChain = nullptr;
		std::vector<vk::raii::ImageView> swapChainImageViews{};
		vk::raii::ImageView depthImageView = nullptr;
		vk::raii::RenderPass renderPass = nullptr;
		std::vector<vk::raii::Framebuffer> framebuffers{};
		std::vector<vk::raii::Semaphore> imageAvailableSemaphores{};
		std::vector<vk::raii::Semaphore> renderFinishedSemaphores{};
		std::vector<vk::raii::Fence> inFlightFences{};
		std::vector<vk::raii::Fence> imagesInFlight{};
	};
}
