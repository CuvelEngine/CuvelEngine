#pragma once
#include "settings/MovementOptions.hpp"

namespace cuvel
{
	enum Directions
	{
		forward = 0,
		backward = 1,
		left = 2,
		right = 3,
		up = 4,
		down = 5
	};

	class Camera
	{
	public:
		void teleport(glm::vec3 pos, glm::vec3 front);
		void updateViewMatrix();

		void updateKeyboardInput(const float_t& dt, const Directions dir);
		void updateMouseInput(const float_t& dt, const double& mouseX, const double& mouseY);

		glm::mat4 viewMatrix{ 1.0f };
	private:
		glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
		glm::vec3 pos{ 0.0f, 0.0f, 1.0f };
		glm::vec3 front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 right{};
		glm::vec3 up{};

		float_t pitch = 0;
		float_t yaw = 0;
		float_t roll = 0;

		double lastMouseX = 0.0;
		double lastMouseY = 0.0;
		double mouseOffsetX = 0.0;
		double mouseOffsetY = 0.0;
		bool   firstMouse = true;
	};
}


