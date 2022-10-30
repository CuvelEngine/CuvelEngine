#pragma once
#include "imgui/ImguiManager.hpp"
#include "settings/MovementOptions.hpp"

namespace cuvel
{
	// This structure is done so directions are shown as a library agnostic tool.
	// Each library will then interpret it as needed. This way glfw doesn't have to be imported
	// here, lowering modularity
	enum Directions
	{
		forward = 0,
		backward = 1,
		left = 2,
		right = 3,
		up = 4,
		down = 5
	};

	// The camera class!! It will simply handle the math and store the resulting viewMatrix that
	// will be used in the shader. Does not handle inputs directly so they have to be given
	// from outside
	class Camera
	{
	public:
		void teleport(glm::vec3 pos, glm::vec3 front);
		void updateViewMatrix();

		void updateKeyboardInput(const float_t& dt, const Directions dir);
		void updateMouseInput(const float_t& dt, const double& mouseX, const double& mouseY);

		void imgui_windows();

		glm::mat4 viewMatrix{ 1.0f };
		bool firstMouse = true;
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

		float mouseSensitivity = 5;
		float movementSpeed = 6;
	};
}


