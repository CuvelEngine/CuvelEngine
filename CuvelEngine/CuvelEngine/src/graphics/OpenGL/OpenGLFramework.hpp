#pragma once
#include <unordered_map>

#include "OpenGLModel.hpp"
#include "graphics/GraphicFramework.hpp"

namespace cuvel
{
	// Class implementation for OpenGL, pretty full of OpenGL boilerplate
	// Still several times simpler than Vulkan's LOL
	// The functionality is the same exact as GraphicFramework, just specific to OpenGL
	// so im not going to explain it
	class OpenGLFramework final : public GraphicFramework
	{
	public:
		explicit OpenGLFramework();
		~OpenGLFramework() override;

		void update(const float_t& dt) override;
		void event(const float_t& dt) override;
		void render() override;

		void addModel(uint32_t id, Mesh mesh) override;

		void setupImgui() override;
		void newFrameImgui() override;
		void destroyImgui() override;

		void imguiWindow() override;

	private:
		// This class manages OpenGL's shaders. Im not sure if this will stay
		// since we can probably just insert it into the class itself, but it works for now
		// #PendingRefactor
		// #ThisIsTotallyNotGoingToStayHereForever
		class Shader
		{
		public:
			// Contrary to what the name of the variable may make you think,
			// this attribute holds the shader's ID
			GLuint id{};

			Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry);

			// Compiles a shader from a file to an OpenGL object
			static uint32_t loadShader(GLenum type, const std::string& file);

			// Simply picks up all shaders together and links them into an OpenGL pipeline
			void linkProgram(uint32_t vs, uint32_t gs, uint32_t fs);
		};

		Shader* shader = nullptr;
		std::unordered_map<uint32_t, OpenGLModel*> models;

		//Imgui parameters
		uint32_t vertices = 0;
		uint32_t indices = 0;
		uint16_t drawCalls = 0;
	};
}

