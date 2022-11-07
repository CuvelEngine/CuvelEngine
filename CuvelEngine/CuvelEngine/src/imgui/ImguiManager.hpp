#pragma once

namespace cuvel
{
	// Class for others to inherit. Must inherit this class for an object to have an
	// associated imGui window
	class ImguiCompatible
	{
	protected:
		~ImguiCompatible() = default;
	public:
		virtual void imguiWindow() = 0;
	};

	// Main ImGui manager class, this class is the one in charge of storing and managing
	// imgui window data.
	class ImguiManager
	{
	public:
		ImguiManager();
		~ImguiManager();

		void addWindow(std::string name, cuvel::ImguiCompatible* baseClass, ImVec2 pos, ImVec2 size);

		void renderImgui();
	private:

		// Internal structure for basic use inside of the class
		// Contains the necessary data for every window
		struct ImGuiWindow
		{
			std::string name;
			cuvel::ImguiCompatible* baseClass;
			bool hook;
			ImVec2 pos;
			ImVec2 size;
		};

		ImGuiIO io;

		std::vector<ImGuiWindow> windows;
	};
}


