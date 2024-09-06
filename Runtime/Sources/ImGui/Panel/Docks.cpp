#include <ImGui/Panels/Docks.h>
#include <Core/MaterialFont.h>

#include <imgui_internal.h>

namespace kbh
{
	Docks::Docks(const MainMenuBar& main_menu_bar) : Panel("Docks"), m_main_menu_bar(main_menu_bar) {}

	void Docks::OnUpdate(ImVec2 size)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });

		ImVec2 work_pos = ImVec2{ 0.f, m_main_menu_bar.GetHeight() };
		ImVec2 work_size = ImVec2{ size.x, size.y - m_main_menu_bar.GetHeight() };

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

				ImGui::DockBuilderDockWindow(KBH_ICON_MD_VIEW_IN_AR" Parameters", dock_left_id);
				ImGui::DockBuilderDockWindow(KBH_ICON_MD_INFO" About", dock_right_id);
				ImGui::DockBuilderDockWindow(KBH_ICON_MD_MONITOR" Render", dock_right_id);
				ImGui::DockBuilderDockWindow(KBH_ICON_MD_SETTINGS" Settings", dock_right_id);
				ImGui::DockBuilderDockWindow(KBH_ICON_MD_TERMINAL" Logs", dock_bottom_id);

				ImGui::DockBuilderFinish(dockspace_id);
			}
			ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2{ 0.f, 0.f }, ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoWindowMenuButton);
			ImGui::End();
		}

		ImGui::PopStyleVar();
	}
}
