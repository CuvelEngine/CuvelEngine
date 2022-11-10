#pragma once

// --------------------------------------------------------------------------
//USE_VULKAN, USE_OPENGL
#define USE_VULKAN
// --------------------------------------------------------------------------

#include <GL/glew.h>

#ifdef USE_VULKAN
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
