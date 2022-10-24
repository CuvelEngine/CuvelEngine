#pragma once
#include "settings/GraphicOptions.hpp"
#include <string>

#include "graphics/Mesh.hpp"

// Simple enumerator for the different library options (DirectX in the future maybe?)
enum GLibrary
{
	OpenGL = 0,
	Vulkan = 1
};

namespace cuvel
{
	//Interface for the Graphics framework, will be inherited by each graphics
	//library implementation.
	// TODO: see if this is actually a good set of functions for the template
	class GraphicFramework
	{
	public:
		virtual ~GraphicFramework() = default;

		// update variables between frames
		virtual void update() = 0;

		// process inputs between frames
		virtual void event() = 0;

		// render new frame
		virtual void render() = 0;

		// add a model to the rendering model buffer
		// Each model must have an ID associated, management left to the implementation
		virtual void addModel(uint32_t id, Mesh mesh) = 0;

		// Check if the window is closed
		int isWindowClosing();

	protected:
		static bool initglfw();
		bool createWindow();
		void initProjection(bool invertedY);

		//TODO: Not sure if Vulkan can use this. May have to move into OpenGLFramework
		static std::string loadShaderSrc(const std::string& file);

		// FrameBuffer sizes, needed to adapt to different aspect ratios
		int32_t fbwidth = 0;
		int32_t fbheight = 0;

		GLFWwindow* window = nullptr;

		// Matrices that are going to be sent to the shaders so the 3D actually makes sense
		glm::mat4 viewMatrix{1.f};
		glm::mat4 projMatrix{1.f};

		// Names are self explanatory. Needed to make the projection matrix
		glm::vec3 cameraPos{};
		glm::vec3 camFront{};
		glm::vec3 worldUp{};
	};
}