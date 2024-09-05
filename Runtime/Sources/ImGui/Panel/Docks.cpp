#include <ImGui/Panels/Docks.h>

#include <imgui_internal.h>

namespace kbh
{
	Docks::Docks() : Panel("Docks") {}

	void Docks::OnUpdate(ImVec2 size)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::SetNextWindowPos({static_cast<float>(size.x) / 4 + 1, 20});
		ImGui::SetNextWindowSize({static_cast<float>(size.x) / 2, static_cast<float>(size.y) / 2.f + 1});
		if(ImGui::Begin("CentralDockSpaceWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking))
		{
			ImGuiID dockspace_id = ImGui::GetID("CentralDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);

			static bool first_time_docks = true;
			if(first_time_docks)
			{
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoWindowMenuButton);
				ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());

				ImGui::DockBuilderDockWindow("About", dockspace_id);

				ImGui::DockBuilderFinish(dockspace_id);

				first_time_docks = false;
			}
			ImGui::End();
		}

		ImGui::PopStyleVar();
	}
}
