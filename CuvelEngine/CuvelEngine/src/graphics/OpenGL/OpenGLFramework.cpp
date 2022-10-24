#include "OpenGLFramework.hpp"

#include <exception>
#include <iostream>

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
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


	void OpenGLFramework::update()
	{
		//TODO: Update camera's ViewMatrix (when we get one)
		glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "ViewMatrix"), 1, false, glm::value_ptr(this->viewMatrix));

		// Update the projection matrix uniform
		glUniformMatrix4fv(glGetUniformLocation(this->shader->id, "ProjectionMatrix"), 1, false, glm::value_ptr(this->projMatrix));
	}

	void OpenGLFramework::event()
	{

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

		// for every model, render it.
		for (const std::pair<uint32_t, OpenGLModel*> model : this->models)
		{
			model.second->render();
		}

		glfwSwapBuffers(window);
		glFlush();

		glfwPollEvents();
	}

	void OpenGLFramework::addModel(uint32_t id, const Mesh mesh)
	{
		// self explanatory
		this->models.emplace(id, new OpenGLModel(mesh, this->shader->id));
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
