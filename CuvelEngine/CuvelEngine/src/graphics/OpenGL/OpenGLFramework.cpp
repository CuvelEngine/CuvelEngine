#include "OpenGLFramework.hpp"

#include <exception>
#include <iostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// You can ignore this, it's a simple callback function for error control
void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

// You can also ignore this, it's just a callback for when the window is resized
void resizeWindow(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
}

namespace cuvel
{
	OpenGLFramework::OpenGLFramework()
	{
		if (!OpenGLFramework::initglfw())
		{
			throw std::exception("Error initiating GLFW");
		}

		// This part configures some stuff for OpenGL before it starts
		// I actually don't get that first one but the rest are pretty self explanatory
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLMAYOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLMINOR);
		glfwWindowHint(GLFW_RESIZABLE, RESIZABLE);
		// Im not sure what this one does either
		glfwWindowHint(GLFW_SAMPLES, GLSAMPLES);


		if (!this->createWindow())
		{
			throw std::exception("Error creating a window with GLFW");
		}

		glfwGetFramebufferSize(window, &fbwidth, &fbheight);
		glfwSetFramebufferSizeCallback(window, resizeWindow);
		//glViewport(0, 0, frameBufferW, frameBufferH);

		// Experimental let's gooooooooooooooo
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			throw std::exception("Error initiating glew");
		}

		// This is so OpenGL tells us when things go wrong
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		// No idea what this does, the depth test thing has to do with depth I think
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);

		// Culling, so only one side of each triangle is rendered. Free fps basically
		glEnable(GL_CULL_FACE);
		// These two lines say: Cull the back faces, front face is counterclockwise
		// (meaning the order in which we put the vertices in the buffer)
		// I feel like we should just flip it, right?
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		// This is for the alpha value, how does OpenGL blend new stuff with
		// stuff behind when they are semitransparent
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// If you change GL_FILL with GL_LINE OpenGL doesn't paint the area inside the triangle
		// It looks very cool, and I guess you can also see the geometry for
		// possible bugs or something, whatever
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// This is what traps the mouse in the window and hides it.
		this->setLockCursor(true);

		this->shader = new Shader(VERTEX_LOCATION, FRAGMENT_LOCATION, GEOMETRY_LOCATION);
		glUseProgram(this->shader->id);

		// --------- INIT MATRICES ---------
		this->initProjection(false);
	}

	OpenGLFramework::~OpenGLFramework()
	{
		// DELETE THIS
		delete this->shader;
	}


	void OpenGLFramework::update(const float_t& dt)
	{
		// Update the view matrix uniform
		glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "ViewMatrix"), 1, false, glm::value_ptr(this->camera.viewMatrix));

		// Update the projection matrix uniform
		glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "ProjectionMatrix"), 1, false, glm::value_ptr(this->projMatrix));
	}

	// For now nothing weird has to be done processing inputs so it just calls the parent function
	void OpenGLFramework::event(const float_t& dt)
	{
		this->GraphicFramework::event(dt);
	}

	void OpenGLFramework::render()
	{
		// Paints the whole frame black again. If you remove this that trippy thing
		// that some games have when you go out of bounds happen, like you start
		// seeing you afterimage. Normally you repaint everything so games don't do it
		// When you go out of bounds they don't have stuff to paint and thus earlier
		// frames are still there
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		this->drawCalls = 0;
		this->vertices = 0;
		this->indices = 0;

		// for every model, render it.
		for (const std::pair<uint32_t, OpenGLModel*> model : this->models)
		{
			model.second->render();
			model.second->getRenderStats(&this->vertices, &this->indices);
			this->drawCalls++;
		}

		//TODO: Isolate this so ImGui is in its own area
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glFlush();
	}

	void OpenGLFramework::addModel(uint32_t id, const Mesh mesh)
	{
		// self explanatory
		this->models.emplace(id, new OpenGLModel(mesh, this->shader->id));
	}

	void OpenGLFramework::setupImgui()
	{
        ImGui_ImplGlfw_InitForOpenGL(window, true);
		const std::string glsl_version = "#version " + std::to_string(GLMAYOR) + std::to_string(GLMINOR) + "0";
		ImGui_ImplOpenGL3_Init(glsl_version.c_str());
	}

	void OpenGLFramework::newFrameImgui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGLFramework::destroyImgui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void OpenGLFramework::imgui_windows()
	{
		ImGui::SetNextWindowSize(ImVec2(250, 120));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Graphics debug");
		ImGui::Text("Frametime: %.3fms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Draw calls: %d", this->drawCalls);
		ImGui::Text("Vertices: %d", this->vertices);
		ImGui::Text("Indices: %d", this->indices);
		ImGui::End();
	}

	OpenGLFramework::Shader::Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry)
	{
		uint32_t gs = 0;

		// create shaders then link them into a pipeline
		const uint32_t vs = loadShader(GL_VERTEX_SHADER, vertex);
		if (!geometry.empty()) gs = loadShader(GL_GEOMETRY_SHADER, geometry);
		const uint32_t fs = loadShader(GL_FRAGMENT_SHADER, fragment);

		OpenGLFramework::Shader::linkProgram(vs, gs, fs);

		// once the pipeline is done we don't need the shader objects anymore
		glDeleteShader(vs);
		if (gs) glDeleteShader(gs);
		glDeleteShader(fs);
	}

	// Both next functions are just all OpenGL overhead, not really worth explaining
	// If you REALLY want to know what is going on you can kind of deduce by reading the
	// function names.
	uint32_t OpenGLFramework::Shader::loadShader(const GLenum type, const std::string& file)
	{
		GLint success;

		const uint32_t shaderID = glCreateShader(type);
		const std::string src = GraphicFramework::loadShaderSrc(file);
		const GLchar* shSrc = src.c_str();
		glShaderSource(shaderID, 1, &shSrc, nullptr);
		glCompileShader(shaderID);

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
			std::cout << infoLog << std::endl;
			throw std::exception("Error while compiling shader");
		}

		return shaderID;
	}

	void OpenGLFramework::Shader::linkProgram(const uint32_t vs, const uint32_t gs, const uint32_t fs)
	{
		GLint success;

		this->id = glCreateProgram();
		glAttachShader(this->id, vs);
		if (gs) glAttachShader(id, gs);
		glAttachShader(this->id, fs);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
			std::cout << infoLog << std::endl;
			glDeleteShader(vs);
			glDeleteShader(fs);
			throw std::exception("Error linking program");
		}
	}
}
