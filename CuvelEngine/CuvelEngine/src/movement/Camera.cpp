#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace cuvel
{
	// change position and the direction where character is looking
	// Maybe we should separate those two so you can just edit one of them?
	void Camera::teleport(glm::vec3 pos, glm::vec3 front)
	{
		this->pos = pos;
		this->front = front;
	}

	// GLM does the heavy lifting for us, look the GLM doc if you wanna know what lookAt does
	void Camera::updateViewMatrix()
	{
		this->viewMatrix = glm::lookAt(this->pos, this->pos + this->front, this->worldUp);
	}

	void Camera::setFrustumInternals(float aspectRatio)
	{
		// store the information
		this->aspectRatio = aspectRatio;

		// compute width and height of the near section
		this->tang = static_cast<float>(tan(glm::radians(FOVy * 0.5)));
	}

	// Pretty self explanatory
	// Front and right are calculated taking into account the camera rotation
	void Camera::updateKeyboardInput(float_t& dt, Directions dir)
	{
		switch (dir)
		{
		case Directions::forward:
			pos += front * this->movementSpeed * dt;
			break;
		case Directions::backward:
			pos -= front * this->movementSpeed * dt;
			break;
		case Directions::right:
			pos += right * this->movementSpeed * dt;
			break;
		case Directions::left:
			pos -= right * this->movementSpeed * dt;
			break;
		case Directions::up:
			pos += worldUp * this->movementSpeed * dt;
			break;
		case Directions::down:
			pos -= worldUp * this->movementSpeed * dt;
			break;
		default:
			break;
		}
	}

	// This one is a bit complicated, but it's basically all trigonometry.
	// Look up how pitch, yaw and roll works to understand it
	//TODO: Prepare for Vulkan (Vulkan uses inverted y axis)
	void Camera::updateMouseInput(float_t& dt, double& mouseX, double& mouseY)
	{
		// If it's the first time it's working we assume there is no movement
		// Otherwise when we open the program the camera will do a weird snap
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

		// We update the pitch and yaw directly from the movement, pretty straightforward
		pitch -= static_cast<float_t>(mouseOffsetY) * this->mouseSensitivity * dt;
		yaw += static_cast<float_t>(mouseOffsetX) * this->mouseSensitivity * dt;

		// We want to limit how much rotation we can get so our head doesn't do a vertical 360
		// and break our necks
		if (pitch > 80.0f)
			pitch = 80.0f;
		else if (pitch < -80.0f)
			pitch = -80.0f;

		// Also we just reset horizontal when it reaches 360, just so we don't end up with a huge
		// rotation number
		if (yaw > 360.0f || yaw < -360.0f)
			yaw = 0.0f;

		// Some complicated math, but it just rotates the front and right vectors
		// according to the pitch and yaw
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	int Camera::pointInFrustum(glm::vec3& point)
	{
		// compute vector from camera position to p
		glm::vec3 v = point - this->pos;

		// compute and test the Z coordinate
		float pcz = glm::dot(v, this->front);
		if (pcz > this->farPlane || pcz < this->nearPlane)
		{
			return(OUTSIDE);
		}
		// compute and test the Y coordinate
		float pcy = glm::dot(v, this->up);
		float aux = pcz * this->tang;
		if (pcy > aux || pcy < -aux)
		{
			return(OUTSIDE);
		}
		// compute and test the X coordinate
		float pcx = glm::dot(v, this->right);
		aux = aux * this->aspectRatio;
		if (pcx > aux || pcx < -aux)
		{
			return(OUTSIDE);
		}
		return(INSIDE);
	}

	void Camera::imguiWindow()
	{
		ImGui::SliderFloat("Mouse sensitivity", &mouseSensitivity, 0.0f, 50.0f);
		ImGui::SliderFloat("Movement Speed", &movementSpeed, 0.0f, 100.0f);
		ImGui::Text("mouse X: %.4f", this->lastMouseX);
		ImGui::Text("mouse Y: %.4f", this->lastMouseY);
		ImGui::Text("mouse offset X: %.4f", this->mouseOffsetX);
		ImGui::Text("mouse offset Y: %.4f", this->mouseOffsetY);
	}
}
