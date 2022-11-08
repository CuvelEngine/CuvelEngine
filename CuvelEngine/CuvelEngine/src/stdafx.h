#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <numeric>
#include <string>


#include <GL/glew.h>
#include <vulkan/vulkan_raii.hpp>

#include "settings/GraphicOptions.hpp"

#ifdef USE_VULKAN
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
