#pragma once

#include "glm/glm.hpp"


namespace cuvel
{
	class Voxel
	{
		 glm::u8vec3 pos;
		 glm::u8vec4 color;

	public:
		Voxel(glm::u8vec3 pos, glm::u8vec4 color) : pos(pos), color(color) {};
	};
}