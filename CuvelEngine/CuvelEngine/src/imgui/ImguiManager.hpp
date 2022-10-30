#pragma once
#include <imgui.h>


namespace cuvel
{
	class ImguiManager
	{
	public:
		explicit ImguiManager();
		~ImguiManager();

		void renderImgui();
	private:
		ImGuiIO io;
	};
}


