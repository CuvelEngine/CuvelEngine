#pragma once
#include "graphics/GraphicFramework.hpp"

namespace cuvel
{
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
	private:

		std::string name = "CuvelEngine";

		//Imgui parameters
		uint32_t vertices = 0;
		uint32_t indices = 0;
		uint16_t drawCalls = 0;
	};


}
