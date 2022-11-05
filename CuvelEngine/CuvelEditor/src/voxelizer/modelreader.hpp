#pragma once

#include "triface.hpp"
#include <string>
#include <vector>


void readObjFile(std::string filePath, std::vector<TriFace> &faces);
