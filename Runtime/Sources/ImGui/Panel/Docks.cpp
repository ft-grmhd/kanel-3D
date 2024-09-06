#include <ImGui/Panels/Docks.h>

#include <imgui_internal.h>

namespace kbh
{
	Docks::Docks() : Panel("Docks") {}

	void Docks::OnUpdate(ImVec2 size)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });

		ImVec2 work_pos = ImVec2{ 0.f, 20.f };
		ImVec2 work_size = ImVec2{ size.x, size.y - 20.f };

		ImGui::SetNextWindowPos(work_pos);
		ImGui::SetNextWindowSize(work_size);
		if(ImGui::Begin("MainDockspaceWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking))
		{
			if(ImGui::DockBuilderGetNode(ImGui::GetID("MainDockspace")) == nullptr)
			{
				ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoWindowMenuButton);
				ImGui::DockBuilderSetNodeSize(dockspace_id, work_size);

				ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 1.f, nullptr, &dockspace_id);
				ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Right, 0.85f, nullptr, &dock_left_id);
				ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down, 0.2f, nullptr, &dock_right_id);

				ImGui::DockBuilderDockWindow("Parameters", dock_left_id);
				ImGui::DockBuilderDockWindow("Render", dock_right_id);
				ImGui::DockBuilderDockWindow("About", dock_right_id);
				ImGui::DockBuilderDockWindow("Logger", dock_bottom_id);

				ImGui::DockBuilderFinish(dockspace_id);
			}
			ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2{ 0.f, 0.f }, ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoWindowMenuButton);
			ImGui::End();
		}

		ImGui::PopStyleVar();
	}
}
