#pragma once
#include <glm/glm.hpp>

namespace cuvel
{
	// The basic structure containing the data for each vertex
	union Vertex
	{
		uint64_t data;
		struct
		{
			glm::u8vec3 position;
			glm::u8vec4 color;
			uint8_t normal;
		};

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && normal == other.normal;
		}
	};
}

// Create a hash function for Vertex struct.
// This is simply so we can use it as key for unordered_map
// Source: https://www.youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR
//
//template <typename T, typename... Rest>
//void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
//	seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
//	(hashCombine(seed, rest), ...);
//};
//
//namespace std {
//	template <>
//	struct hash<cuvel::Vertex> {
//		size_t operator()(cuvel::Vertex const& vertex) const {
//			size_t seed = 0;
//			hashCombine(seed, vertex.position, vertex.color, vertex.normal);
//			return seed;
//		}
//	};
//}
