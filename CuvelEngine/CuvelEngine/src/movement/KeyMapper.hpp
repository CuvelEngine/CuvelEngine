#pragma once
#include <unordered_map>

namespace cuvel
{
	class GraphicFramework;
	class KeyMapper;

	// Weird hack to be able to use non-static methods as pointers
	typedef void (KeyMapper::*KeyMapAction) (std::string&, std::string&, float);

	// Main class in charge of handling keymappings and executing its behavior.
	// Potentially the class with the most capability to destroy modularity :P
	// Let's try to avoid that for now tho
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
		void move(std::string& param1, std::string& param2, float dt);

		// Param1 -> unused
		// Param2 -> unused
		void lockCursor(std::string& param1, std::string& param2, float dt);

		// ---------------------------------------------------------------------------

	private:
		// Structure with the basic keymappings
		struct KeyMap
		{
			KeyMapAction action;
			std::string param1;
			std::string param2;
			bool canBeHeld;
			bool released;
		};

		// External dependencies
		// ---------------------------------------------------------------------------
		GraphicFramework* gFram = nullptr;

		// ---------------------------------------------------------------------------

		// Each keymapping is linked to the keyboard input as key
		std::unordered_map<int, KeyMap> keyMaps;
	};
}

