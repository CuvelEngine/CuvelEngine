#include "VoxelReader.hpp"

#include "Voxel.hpp"
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <fstream>

namespace cuvel 
{
	void readVoxmFile(
		std::string& filePath, 
		std::vector<Voxel>* voxels,
		cuvel::FlagVector* neighborFlags,
		glm::u8vec3* size, 
		uint32_t* vertices, uint32_t* indices)
	{
		std::ifstream input(filePath, std::ifstream::binary);
		if (!input) 
		{
			std::cout << "Couldn't read file " << filePath << std::endl;
			return;
		}

		// Read model size
		glm::uint8 model_size[3];
		input.read(reinterpret_cast<char*>(model_size), 3);
		*size = glm::u8vec3(model_size[0], model_size[1], model_size[2]);

		neighborFlags->resize(*size, false);

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
			voxels->emplace_back(current_pos, glm::u8vec4(rgb, a));

			if (a == 255)
			{
				neighborFlags->setFlag(current_pos, *size, true);
			}

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
		*vertices = vertex_count;
		*indices = index_count;
		return;
	}

	void generateVoxmSample(
		std::vector<Voxel>* voxels,
		cuvel::FlagVector* neighborFlags,
		glm::u8vec3* size,
		uint32_t* vertices, uint32_t* indices)
	{
		*size = glm::u8vec3(254, 254, 254);
		neighborFlags->resize(*size, false);
		
		for (uint8_t z = 0; z < size->z; z++)
		{
			for (uint8_t y = 0; y < size->y; y++)
			{
				for (uint8_t x = 0; x < size->x; x++)
				{
					glm::u8vec3 pos(x, y, z);
					if (pos.x == 0 || pos.y == 0 || pos.z == 0 || (pos.x == pos.y && pos.y == pos.z))
					{
						voxels->emplace_back(pos, glm::u8vec4(255, 255, 255, 255));
						neighborFlags->setFlag(pos, *size, true);
					}
				}
			}
		}
		*vertices = 0;
		*indices = 0;
	}
}

