#include "KeyMapper.hpp"
#include "graphics/GraphicFramework.hpp"

// Basic mapping that translates from text to enum. Very simple
static std::unordered_map<std::string, cuvel::Directions> dirTable = {
	{"backward", cuvel::Directions::backward},
	{"forward", cuvel::Directions::forward},
	{"right", cuvel::Directions::right},
	{"left", cuvel::Directions::left},
	{"up", cuvel::Directions::up},
	{"down", cuvel::Directions::down}
};

namespace cuvel
{
	KeyMapper::KeyMapper(GraphicFramework* gFram)
	: gFram(gFram){}

	void KeyMapper::addKeyMap(int key, KeyMapAction action, bool canBeHeld, std::string param1, std::string param2)
	{
		if (this->keyMaps.contains(key))
		{
			std::string excepStr = "Duplicated entry for key " + key;
			throw std::exception(excepStr.c_str());
		}
		KeyMap keyMap = { action, std::move(param1), std::move(param2), canBeHeld, false };
		this->keyMaps.emplace(key, keyMap);
	}

	void KeyMapper::executeKeyMaps(float dt)
	{
		// Simply go through each relevant key and edit the camera if it is triggered
		for (auto& [key, map]: KeyMapper::keyMaps)
		{
			// The key must be pressed. If the mapping is anti-holding and it's been held
			// we avoid executing the function
			if (this->gFram->isKeyPressed(key) && (map.canBeHeld || map.released))
			{
				// This weird thing calls the function pointer through the KeyMapper object
				(this->*map.action)(map.param1, map.param2, dt);
				map.released = false;
			}
			// When the user releases the key we can allow the function to be called in the
			// next press
			else if (!this->gFram->isKeyPressed(key))
			{
				map.released = true;
			}
		}
	}

	// Moves the camera in a specific direction
	void KeyMapper::move(std::string& param1, std::string& param2, float dt)
	{
		this->gFram->camera.updateKeyboardInput(dt, dirTable.at(param1));
	}

	// Locks or unlocks the cursor so it can either be catched by the game or moved
	// around the computer
	void KeyMapper::lockCursor(std::string& param1, std::string& param2, float dt)
	{
		this->gFram->setLockCursor();
	}
}
