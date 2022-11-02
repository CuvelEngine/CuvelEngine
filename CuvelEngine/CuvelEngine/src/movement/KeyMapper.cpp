#include "KeyMapper.hpp"

#include "graphics/GraphicFramework.hpp"

// Basic mapping that translates from text to enum. Very simple
static std::unordered_map<std::string, cuvel::Directions> const dirTable = {
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

	void KeyMapper::addKeyMap(int key, const KeyMapAction action, const bool canBeHeld, std::string param1, std::string param2)
	{
		cuvel::KeyMap keyMap = { action, std::move(param1), std::move(param2), canBeHeld, false };
		this->keyMaps.emplace(key, keyMap);
	}

	void KeyMapper::executeKeyMaps(const float dt)
	{
		// Simply go through each relevant key and edit the camera if it is triggered
		for (auto& [key, map]: KeyMapper::keyMaps)
		{
			if (this->gFram->isKeyPressed(key) && (map.canBeHeld || map.released))
			{
				(this->*map.action)(map.param1, map.param2, dt);
				map.released = false;
			}
			else if (!this->gFram->isKeyPressed(key))
			{
				map.released = true;
			}
		}
	}

	// Moves the camera in a specific direction
	void KeyMapper::move(const std::string& param1, const std::string& param2, const float dt)
	{
		this->gFram->camera.updateKeyboardInput(dt, dirTable.at(param1));
	}

	// Locks or unlocks the cursor so it can either be catched by the game or moved
	// around the computer
	void KeyMapper::lockCursor(const std::string& param1, const std::string& param2, const float dt)
	{
		this->gFram->setLockCursor();
	}
}
