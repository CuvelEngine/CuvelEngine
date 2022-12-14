#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>

// --------------------------------------------------------------------------

// window
#define RESIZABLE true
#define CLEARCOLOR 0.1f, 0.1f, 0.3f

// Projection
#define INITIAL_FOV 90.0f
#define NEAR_PLANE 0.2f
#define FAR_PLANE 10000.0f

// Shaders
#define VERTEX_LOCATION "src/graphics/shaders/vertex.glsl"
#define FRAGMENT_LOCATION "src/graphics/shaders/fragment.glsl"
#define GEOMETRY_LOCATION ""

// OPENGL
#define GLSTOPERROR true

#define GLMAYOR 4
#define GLMINOR 5
#define GLSAMPLES 16