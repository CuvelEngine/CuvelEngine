#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>

namespace cuvel
{
	class FlagVector
	{
	public:
		void resize(glm::u8vec3 size, bool defval)
		{
			this->resize(static_cast<size_t>(size.x) * size.y * size.z, defval);
		}

		void resize(size_t size, bool defVal)
		{
#ifndef NDEBUG
			if (!this->flags.empty())std::cout << "Warning!! Resizing non-empty flag vector!!\n";
#endif
			size_t newSize = std::ceil(size / 8.0);
			this->flags.resize(newSize, defVal);
		}

		void setFlag(size_t pos, bool val)
		{
			size_t index = std::floor(pos / 8.0);
			uint8_t offset = pos % 8;
			if (val)
			{
				this->flags[index] |= FlagVector::onValues[offset];
			}
			else
			{
				this->flags[index] &= FlagVector::offValues[offset];
			}
		}

		void setFlag(glm::u8vec3 pos, glm::u8vec3 size, bool val)
		{
			this->setFlag(FlagVector::getIndexFromCoord(pos, size), val);
		}

		bool getFlag(size_t pos)
		{
			size_t index = std::floor(pos / 8.0);
			uint8_t offset = pos % 8;
#ifndef NDEBUG
			if (index >= this->flags.size())
			{
				std::cout << "Warning!! Attempted to access flagVector of size " << this->flags.size() << " at index " << pos << "\n";
			}
#endif
			return (this->flags.at(index) & FlagVector::onValues[offset]) == FlagVector::onValues[offset];
		}

		bool getFlag(glm::u8vec3 pos, glm::u8vec3 size)
		{
			return this->getFlag(FlagVector::getIndexFromCoord(pos, size));
		}

	private:
		std::vector<uint8_t> flags;
		inline static uint8_t onValues[8] = {
			0x01, 0x02, 0x04, 0x08,
			0x10, 0x20, 0x40, 0x80
		};
		inline static uint8_t offValues[8] = {
			0xFE, 0xFD, 0xFB, 0xF7,
			0xEF, 0xDF, 0xBF, 0x7F
		};

		static size_t getIndexFromCoord(glm::u8vec3 pos, glm::u8vec3 size)
		{
			return static_cast<size_t>(pos.x) +
				(static_cast<size_t>(pos.y) * size.x) +
				(static_cast<size_t>(pos.z) * size.y * size.x);
		}
	};
}


