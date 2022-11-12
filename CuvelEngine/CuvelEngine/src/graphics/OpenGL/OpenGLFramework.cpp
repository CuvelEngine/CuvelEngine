#include "OpenGLFramework.hpp"

#include <exception>
#include <iostream>
#include <cmath>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

// You can ignore this, it's a simple callback function for error control
void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "---------------------opengl-callback-start------------" << std::endl;
	std::cout << "message: " << message << std::endl;
	std::cout << "source: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "API";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "OTHER";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "SHADER COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "THIRD PARTY";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "WINDOW SYSTEM";
		break;
	}
	std::cout << std::endl;

	std::cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
		break;
	}
	std::cout << std::endl;

	std::cout << "id: " << id << std::endl;
	std::cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "NOTIFICATION";
		break;
	default:
		std::cout << "UNKNOWN (" << severity <<")";
	}
	std::cout << std::endl;
	std::cout << "---------------------opengl-callback-end--------------" << std::endl;
	if (GLSTOPERROR && type == GL_DEBUG_TYPE_ERROR)
	{
		__debugbreak();
	}
}

float aspectRatio;

// You can also ignore this, it's just a callback for when the window is resized
void resizeWindow(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
	aspectRatio = static_cast<float>(fbw) / fbh;
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
		//glfwWindowHint(GLFW_SAMPLES, GLSAMPLES);


		if (!this->createWindow())
		{
			throw std::exception("Error creating a window with GLFW");
		}

		glfwGetFramebufferSize(window, &fbwidth, &fbheight);
		glfwSetFramebufferSizeCallback(window, resizeWindow);
		aspectRatio = static_cast<float>(fbwidth) / fbheight;
		this->camera.setFrustumInternals(aspectRatio);

		//glViewport(0, 0, frameBufferW, frameBufferH);

		// Experimental let's gooooooooooooooo
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			throw std::exception("Error initiating glew");
		}

		// This is so OpenGL tells us when things go wrong
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Disable the notification severity level, we only care about warnings and errors
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		
		//glEnable(GL_MULTISAMPLE);
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// This is what traps the mouse in the window and hides it.
		this->setLockCursor();

		this->shader = new Shader(VERTEX_LOCATION, FRAGMENT_LOCATION, GEOMETRY_LOCATION);
		glUseProgram(this->shader->id);

		// --------- INIT MATRICES ---------
		this->initProjection(false);
	}

	OpenGLFramework::~OpenGLFramework()
	{
		delete this->shader;
	}


	void OpenGLFramework::update(float_t& dt)
	{
		if (aspectRatio != this->camera.aspectRatio)
		{
			this->camera.setFrustumInternals(aspectRatio);
		}
		if (Camera::lastFOV != Camera::FOVy)
		{
			this->updateProjection();
			this->camera.setFrustumInternals(aspectRatio);
			Camera::lastFOV = Camera::FOVy;
		}

		// Update the view matrix uniform
		glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "ViewMatrix"), 1, false, glm::value_ptr(this->camera.viewMatrix));

		// Update the projection matrix uniform
		glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "ProjectionMatrix"), 1, false, glm::value_ptr(this->projMatrix));

		// Update the projection matrix uniform
		glUniform3fv(glGetUniformLocation(this->shader->id, "lightDir"), 1, glm::value_ptr(this->lightDir));

	}

	// For now nothing weird has to be done processing inputs so it just calls the parent function
	void OpenGLFramework::event(KeyMapper* keyMapper, float_t& dt)
	{
		this->GraphicFramework::event(keyMapper, dt);
	}

	void OpenGLFramework::render()
	{
		// Paints the whole frame black again. If you remove this that trippy thing
		// that some games have when you go out of bounds happen, like you start
		// seeing you after image. Normally you paint everything with skyboxes and models
		// so games don't do it. But when you go out of bounds they don't have stuff to paint
		// and thus earlier frames are still there
		glClearColor(CLEARCOLOR, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		this->drawCalls = 0;
		this->vertices = 0;
		this->indices = 0;

		// for every model, render it.
		for (std::pair<uint32_t, OpenGLModel*> model : this->models)
		{
			if (model.second->isInsideClippingPlane(&this->camera))
			{
				model.second->render();
				// This is done to gather some stats about the object for the ImGui debug
				model.second->getRenderStats(&this->vertices, &this->indices);
				this->drawCalls++;
			}
			
		}

		//TODO: Isolate this so ImGui is in its own area
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glFlush();
	}

	void OpenGLFramework::addModel(uint32_t id, std::string& mesh, bool hasLighting, glm::vec3 pos)
	{
		if (!this->meshes.contains(mesh))
		{
			throw std::runtime_error("Tried to create a model with non-existing mesh");
		}
		this->models.emplace(id, new OpenGLModel(this->meshes.at(mesh), this->shader->id, hasLighting));
		this->models.at(id)->translate(pos);
	}

	std::shared_ptr<Mesh> OpenGLFramework::addMesh(std::string& filename)
	{
		if (this->meshes.contains(filename))
		{
			std::cout << "Tried to add mesh " << filename << " that is already in!!" << "\n";
			return this->meshes.at(filename);
		}
		std::shared_ptr<OpenGLMesh> mesh = std::make_shared<OpenGLMesh>();
		this->meshes.emplace(filename, mesh);
		return mesh;
	}

	void OpenGLFramework::destroyMesh(std::string& filename)
	{
		if (!this->meshes.contains(filename)) return;
		this->meshes.erase(filename);
	}

	void OpenGLFramework::registerMeshBuffers(std::string& mesh)
	{
		if (!this->meshes.contains(mesh))
		{
			throw std::runtime_error("Tried to register non-existing mesh");
		}
		std::dynamic_pointer_cast<OpenGLMesh>(this->meshes.at(mesh))->registerBuffers(this->shader->id);
	}

	void OpenGLFramework::setupImgui()
	{
        ImGui_ImplGlfw_InitForOpenGL(window, true);
		std::string glsl_version = "#version " + std::to_string(GLMAYOR) + std::to_string(GLMINOR) + "0";
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

	void OpenGLFramework::imguiWindow()
	{
		ImGui::Text("Frametime: %.3fms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Text("Draw calls: %d", this->drawCalls);
		ImGui::Text("Vertices: %d", this->vertices);
		ImGui::Text("Indices: %d", this->indices);
		ImGui::Separator();
		ImGui::SliderFloat3("Light direction", glm::value_ptr(this->lightDir), -1, 1);
		ImGui::SliderFloat("FOV", &cuvel::Camera::FOVy, 40, 150, "%.0fº");
		ImGui::Separator();
		ImGui::Text("Position: (%.3f, %.3f, %.3f)", this->camera.pos.x, this->camera.pos.y, this->camera.pos.z);
		ImGui::Text("Looking at: (%.3f, %.3f, %.3f)", this->camera.front.x, this->camera.front.y, this->camera.front.z);
	}

	OpenGLFramework::Shader::Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry)
	{
		uint32_t gs = 0;

		// create shaders then link them into a pipeline
		uint32_t vs = loadShader(GL_VERTEX_SHADER, vertex);
		if (!geometry.empty()) gs = loadShader(GL_GEOMETRY_SHADER, geometry);
		uint32_t fs = loadShader(GL_FRAGMENT_SHADER, fragment);

		OpenGLFramework::Shader::linkProgram(vs, gs, fs);

		// once the pipeline is done we don't need the shader objects anymore
		glDeleteShader(vs);
		if (gs) glDeleteShader(gs);
		glDeleteShader(fs);
	}

	// Both next functions are just all OpenGL overhead, not really worth explaining
	// If you REALLY want to know what is going on you can kind of deduce by reading the
	// function names.
	uint32_t OpenGLFramework::Shader::loadShader(GLenum type, const std::string& file)
	{
		GLint success;

		uint32_t shaderID = glCreateShader(type);
		std::string src = GraphicFramework::loadShaderSrc(file);
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

	void OpenGLFramework::Shader::linkProgram(uint32_t vs, uint32_t gs, uint32_t fs)
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
