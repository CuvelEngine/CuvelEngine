#include "ImguiManager.hpp"

#include <utility>


namespace cuvel
{
	// Must be executed before anything else (I think)
	ImguiManager::ImguiManager()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		this->io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();
	}
	ImguiManager::~ImguiManager()
	{
		ImGui::DestroyContext();
	}

	void ImguiManager::addWindow(std::string name, cuvel::ImguiCompatible* baseClass, const ImVec2 pos, const ImVec2 size)
	{
		const ImGuiWindow win = {std::move(name), baseClass, false, pos, size };
		this->windows.push_back(win);
	}

	void ImguiManager::renderImgui()
	{
		// Create a menu in a local context (Just for readability)
		{
			ImGui::BeginMainMenuBar();
			// Obtain the desired width of each button depending on how many there are
			//TODO: Maybe get a better padding system so buttons look better. Not important
			const ImVec2 viewPortSize = ImGui::GetMainViewport()->Size;
			const float sizePerButton = (viewPortSize.x / this->windows.size()) - 13;

			// Create a menu button for each window and hook it to the associated bool
			for (auto& [name, baseClass, hook, pos, size] : this->windows)
			{
				if (ImGui::Button(name.c_str(), ImVec2(sizePerButton, 20)))
					hook = !hook;
			}
			ImGui::EndMainMenuBar();
		}

		for (auto& [name, baseClass, hook, pos, size] : this->windows)
		{
			if (hook)
			{
				// Set position and size
				ImGui::SetNextWindowSize(size);
				ImGui::SetNextWindowPos(pos);

				// Create window using given class's inherited function
				ImGui::Begin(name.c_str(), &hook);
				baseClass->imguiWindow();
				ImGui::End();
			}
		}

		ImGui::Render();
	}
}
