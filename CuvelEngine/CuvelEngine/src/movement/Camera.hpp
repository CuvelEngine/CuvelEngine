#pragma once

#include "imgui/ImguiManager.hpp"
#include "settings/GraphicOptions.hpp"

namespace cuvel
{

	enum FrustResult
	{
		OUTSIDE,
		INTERSECT,
		INSIDE
	};

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
	class Camera : public ImguiCompatible
	{
	public:
		void teleport(glm::vec3 pos, glm::vec3 front);
		void updateViewMatrix();
		void setFrustumInternals(float aspectRatio);

		void updateKeyboardInput(float_t& dt, Directions dir);
		void updateMouseInput(float_t& dt, double& mouseX, double& mouseY);

		int pointInFrustum(glm::vec3& point);

		void imguiWindow() override;

		glm::mat4 viewMatrix{ 1.0f };
		bool firstMouse = true;

		// Camera vectors
		glm::vec3 pos{ -5.0f, 3.0f, 5.0f };
		glm::vec3 front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
		glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
		glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		// FOV and AR
		inline static float FOVy = INITIAL_FOV;
		inline static float lastFOV = INITIAL_FOV;
		float aspectRatio{};

	private:
		float_t pitch = 0;
		float_t yaw = 0;

		double lastMouseX = 0.0;
		double lastMouseY = 0.0;
		double mouseOffsetX = 0.0;
		double mouseOffsetY = 0.0;

		float mouseSensitivity = 5;
		float movementSpeed = 6;

		// Frustum data
		float nearPlane = NEAR_PLANE;
		float farPlane = FAR_PLANE;
		float tang{};
	};
}


