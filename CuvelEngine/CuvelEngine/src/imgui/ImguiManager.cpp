#include "ImguiManager.hpp"


namespace cuvel
{
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
}
