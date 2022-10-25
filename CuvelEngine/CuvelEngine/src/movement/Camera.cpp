#include "Camera.hpp"

#include <GL/glew.h>


namespace cuvel
{
	void Camera::teleport(const glm::vec3 pos, const glm::vec3 front)
	{
		this->pos = pos;
		this->front = front;
	}

	void Camera::updateViewMatrix()
	{
		this->viewMatrix = glm::lookAt(this->pos, this->pos + this->front, this->worldUp);
	}

	void Camera::updateKeyboardInput(const float_t& dt, const Directions dir)
	{
		switch (dir)
		{
		case Directions::forward:
			pos += front * MOVEMENT_SPEED * dt;
			break;
		case Directions::backward:
			pos -= front * MOVEMENT_SPEED * dt;
			break;
		case Directions::left:
			pos -= right * MOVEMENT_SPEED * dt;
			break;
		case Directions::right:
			pos += right * MOVEMENT_SPEED * dt;
			break;
		case Directions::up:
			pos += worldUp * MOVEMENT_SPEED * dt;
			break;
		case Directions::down:
			pos -= worldUp * MOVEMENT_SPEED * dt;
			break;
		default:
			break;
		}
	}

	void Camera::updateMouseInput(const float_t& dt, const double& mouseX, const double& mouseY)
	{
		if (firstMouse)
		{
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			firstMouse = false;
		}

		mouseOffsetX = mouseX - lastMouseX;
		mouseOffsetY = mouseY - lastMouseY;
		lastMouseX = mouseX;
		lastMouseY = mouseY;

		pitch -= static_cast<float_t>(mouseOffsetY) * MOUSE_SENSITIVITY * dt;
		yaw += static_cast<float_t>(mouseOffsetX) * MOUSE_SENSITIVITY * dt;

		if (pitch > 80.0f)
			pitch = 80.0f;
		else if (pitch < -80.0f)
			pitch = -80.0f;

		if (yaw > 360.0f || yaw < -360.0f)
			yaw = 0.0f;

		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
}
