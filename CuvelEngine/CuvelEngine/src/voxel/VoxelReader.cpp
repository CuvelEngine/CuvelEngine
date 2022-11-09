#include "VoxelReader.hpp"
#include <iostream>
#include <fstream>


namespace cuvel 
{
	std::vector<Voxel> readVoxmFile(std::string filePath)
	{
		std::vector<Voxel> voxels;

		std::ifstream input(filePath, std::ifstream::binary);
		if (!input) 
		{
			std::cout << "Couldn't read file" << std::endl;
			return voxels;
		}

		// Read model size
		glm::uint8 model_size[3];
		input.read(reinterpret_cast<char*>(model_size), 3);

		// Read vertex and index count
		glm::uint32 vertex_count, index_count;
		input.read(reinterpret_cast<char*>(&vertex_count), 4);
		input.read(reinterpret_cast<char*>(&index_count), 4);

		// Iterate through voxels
		glm::u8vec3 current_pos(0, 0, 0);
		glm::u8vec3 rgb;
		glm::uint8 a;

		while (true)
		{
			// Read A (or 0x0 if air voxel)
			input.read(reinterpret_cast<char*>(&a), 1);
			if (input.eof()) break;

			// If air voxel, read new coords for next solid voxel and A value
			if (a == 0x00) 
			{
				input.read(reinterpret_cast<char*>(&current_pos), 3);
				input.read(reinterpret_cast<char*>(&a), 1);
			}

			// Read RGB values
			input.read(reinterpret_cast<char*>(&rgb), 3);

			// Build voxel
			voxels.push_back(Voxel(current_pos, glm::u8vec4(rgb, a)));

			// Update position taking into account the model size
			current_pos.x = (current_pos.x + 1) % model_size[0];
			if (current_pos.x == 0) 
			{
				current_pos.y = (current_pos.y + 1) % model_size[1];
				if (current_pos.y == 0)
				{
					current_pos.z = (current_pos.z + 1) % model_size[2];
				}
			}
		}

		input.close();
		return voxels;
	}
}

