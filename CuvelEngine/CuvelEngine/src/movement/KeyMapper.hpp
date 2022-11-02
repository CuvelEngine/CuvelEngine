#pragma once
#include <string>
#include <unordered_map>
namespace cuvel
{
	class GraphicFramework;
	class KeyMapper;

	typedef void (KeyMapper::*KeyMapAction) (const std::string&, const std::string&, float);

	struct KeyMap
	{
		KeyMapAction action;
		std::string param1;
		std::string param2;
		bool canBeHeld;
		bool released;
	};

	class KeyMapper
	{
	public:
		explicit KeyMapper(GraphicFramework* gFram);
		void addKeyMap(
			int key, 
			KeyMapAction action, 
			bool canBeHeld, 
			std::string param1 = "", 
			std::string param2 = ""
		);
		void executeKeyMaps(float dt);
		
		// Add your own actions as you need. They must always have the same arguments
		// ---------------------------------------------------------------------------

		// Param1 -> direction name, follows the same words as Camera enum
		// Param2 -> unused
		void move(const std::string& param1, const std::string& param2, float dt);

		// Param1 -> unused
		// Param2 -> unused
		void lockCursor(const std::string& param1, const std::string& param2, float dt);

		// ---------------------------------------------------------------------------

	private:
		GraphicFramework* gFram = nullptr;
		std::unordered_map<int, KeyMap> keyMaps;
	};
}

