#pragma once

// --------------------------------------------------------------------------
//#define USE_OPENGL
#define USE_OPENGL
// --------------------------------------------------------------------------

#include <GL/glew.h>
#include <vulkan/vulkan_raii.hpp>

#ifdef USE_VULKAN
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
