#pragma once
#include "graphics/Mesh.hpp"
#include "movement/Camera.hpp"

// Simple enumerator for the different library options (DirectX in the future maybe?)
enum GLibrary
{
	OpenGL = 0,
	Vulkan = 1
};

namespace cuvel
{
	class KeyMapper;

	//Interface for the Graphics framework, will be inherited by each graphics
	//library implementation.
	// TODO: see if this is actually a good set of functions for the template
	class GraphicFramework : public ImguiCompatible
	{
	public:
		virtual ~GraphicFramework();

		// update variables between frames
		virtual void update(float_t& dt) = 0;

		// process inputs between frames
		virtual void event(KeyMapper* keyMapper, float_t& dt);

		// render new frame
		virtual void render() = 0;

		// add a model to the rendering model buffer
		// Each model must have an ID associated, management left to the implementation
		virtual void addModel(uint32_t id, Mesh mesh, bool hasLighting, glm::vec3 pos) = 0;

		// Imgui handling
		virtual void setupImgui() = 0;
		virtual void newFrameImgui() = 0;
		virtual void destroyImgui() = 0;

		void imguiWindow() override = 0;

		// ------------------------------------------------------------------------
		// ----------------------- End of virtual functions -----------------------
		// ------------------------------------------------------------------------

		// Check if the window is closed
		int isWindowClosing();
		bool isKeyPressed(int key);
		void setLockCursor();
		
		// Camera class that will beautifully handle the viewMatrix
		Camera camera;
	protected:
		static bool initglfw();
		bool createWindow();
		void initProjection(bool invertedY);
		void updateProjection();

		//TODO: Not sure if Vulkan can use this. May have to move into OpenGLFramework
		static std::string loadShaderSrc(const std::string& file);

		// FrameBuffer sizes, needed to adapt to different aspect ratios
		int32_t fbwidth = 0;
		int32_t fbheight = 0;

		// Matrix that is going to be sent to the shaders so the 3D space actually makes sense
		glm::mat4 projMatrix{1.f};

		// Light source
		glm::vec3 lightDir{ -0.41f, 0.59f, 1.f };

		GLFWwindow* window = nullptr;
	};
}