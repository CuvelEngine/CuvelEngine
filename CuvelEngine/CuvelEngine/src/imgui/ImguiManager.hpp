#pragma once
#include <imgui.h>


namespace cuvel
{
	class ImguiManager
	{
	public:
		ImguiManager();
		~ImguiManager();

		void renderImgui();

	private:
		// Create variables here
		//------------------------------------------------
		float mouseSensitivity = 6.f;
		float movementSpeed = 5.f;

		//------------------------------------------------

		ImGuiIO io;
	};
}


