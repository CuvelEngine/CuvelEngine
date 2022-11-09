#pragma once

#include <string>
#include <vector>
#include "Voxel.hpp"

namespace cuvel
{
	std::vector<Voxel> readVoxmFile(std::string filePath);
}